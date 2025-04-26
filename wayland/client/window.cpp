
#include "window.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <optional>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <GLES3/gl3.h>
#include "xdg-shell-client.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static auto print_fps() -> void
{
    using namespace std::chrono;

    static int frames = 0;
    static std::optional<high_resolution_clock::time_point> previous_time {};

    if (!previous_time) {
        previous_time = high_resolution_clock::now();
        return;
    }

    ++frames;

    auto current_time = high_resolution_clock::now();
    auto elapsed_time_ms = duration_cast<milliseconds>(current_time - *previous_time);
    if (elapsed_time_ms.count() >= 1000) {
        std::cout << "FPS: " << frames << std::endl;
        frames = 0;
        previous_time = current_time;
    }
}

static auto frame_done(void *data, struct wl_callback* callback, uint32_t time) -> void;

struct wl_callback_listener wl_surface_frame_listener = {
    .done = frame_done,
};

static auto frame_done(void* data, struct wl_callback* callback, uint32_t time) -> void
{
    auto window = reinterpret_cast<Window*>(data);
    window->frame_done(callback, time);
}

static auto configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial) -> void
{
    auto window = reinterpret_cast<Window*>(data);
    window->configure(xdg_surface, serial);
}

struct xdg_surface_listener xdg_surface_listener = {
    .configure = configure,
};

const char* vertex_shader_source = R"(
#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragment_shader_source = R"(
#version 300 es
precision mediump float;

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
}
)";

Window::Window(Display& display, int width, int height)
{
    m_wl_surface = wl_compositor_create_surface(display.wl_compositor());

    m_xdg_surface = xdg_wm_base_get_xdg_surface(display.xdg_wm_base(), m_wl_surface);
    xdg_surface_add_listener(m_xdg_surface, &xdg_surface_listener, this);

    m_xdg_toplevel = xdg_surface_get_toplevel(m_xdg_surface);
    xdg_toplevel_set_title(m_xdg_toplevel, "Hello, Wayland!");
    wl_surface_commit(m_wl_surface);

    m_wl_egl_window = wl_egl_window_create(m_wl_surface, width, height);

    m_egl_display = display.egl_display();

    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLConfig egl_config;
    EGLint num_config;

    eglChooseConfig(m_egl_display, config_attribs, &egl_config, 1, &num_config);
    assert(num_config > 0);

    m_egl_surface = eglCreatePlatformWindowSurface(m_egl_display, egl_config, m_wl_egl_window, NULL);

    EGLint context_attribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_NONE
    };
    m_egl_context = eglCreateContext(m_egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);

    eglMakeCurrent(m_egl_display, m_egl_surface, m_egl_surface, m_egl_context);

    glViewport(0, 0, width, height);

    unsigned int vertex_shadrer = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shadrer, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shadrer);
#ifndef NDEBUG
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shadrer, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shadrer, 512, NULL, info_log);
        std::cerr << "Vertex shader compilation failed:\n" << info_log << std::endl;
    }
#endif

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
#ifndef NDEBUG
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cerr << "Fragment shader compilation failed:\n" << info_log << std::endl;
    }
#endif

    m_program = glCreateProgram();
    glAttachShader(m_program, vertex_shadrer);
    glAttachShader(m_program, fragment_shader);
    glLinkProgram(m_program);
#ifndef NDEBUG
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_program, 512, NULL, info_log);
        std::cerr << "Shader program linking failed:\n" << info_log << std::endl;
    }
#endif

    glUseProgram(m_program);

    glDeleteShader(vertex_shadrer);
    glDeleteShader(fragment_shader);

    float vertices[] = {
        // positioins       // texture coordinates
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int tex_width, tex_height, tex_channels;
    unsigned char* data = stbi_load("../container.jpg", &tex_width, &tex_height, &tex_channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    } else {
        std::cerr << "Texture loading failed" << std::endl;
    }
    stbi_image_free(data);
}

Window::~Window()
{
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_ebo);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &m_texture);

    glUseProgram(0);
    glDeleteProgram(m_program);

    if (eglGetCurrentContext() == m_egl_context) {
        eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
    eglDestroyContext(m_egl_display, m_egl_context);
    eglDestroySurface(m_egl_display, m_egl_surface);

    wl_egl_window_destroy(m_wl_egl_window);
    xdg_toplevel_destroy(m_xdg_toplevel);
    xdg_surface_destroy(m_xdg_surface);
    wl_surface_destroy(m_wl_surface);
}

auto Window::configure(struct xdg_surface* xdg_surface, uint32_t serial) -> void
{
    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_callback* callback = wl_surface_frame(m_wl_surface);
    wl_callback_add_listener(callback, &wl_surface_frame_listener, this);

    draw();
}

auto Window::draw() -> void
{
    static GLfloat color[4] = { 0.0, 1.0, 0.0, 1.0 };
    static int inc = 0, dec = 1;

    color[inc] += 0.01;
    color[dec] -= 0.01;

    if (color[dec] <= 0.0) {
        dec = inc;
        inc = (inc + 2) % 3;
    }

    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_program);
    glBindVertexArray(m_vao);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    eglSwapBuffers(m_egl_display, m_egl_surface);

#ifndef NDEBUG
    print_fps();
#endif
}

auto Window::frame_done(struct wl_callback* callback, uint32_t time) -> void
{
	wl_callback_destroy(callback);
    callback = wl_surface_frame(m_wl_surface);
	wl_callback_add_listener(callback, &wl_surface_frame_listener, this);

    draw();
}
