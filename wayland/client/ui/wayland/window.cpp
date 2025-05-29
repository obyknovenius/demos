
#include "window.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <optional>
#include <GLES3/gl3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "server.h"
#include "../window.h"
#include "../layer.h"

namespace wayland {

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

const char* vertex_shader_source = R"(
#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * transform * vec4(aPos, 1.0);
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

Window::Window(Server& server, const gfx::Size& size)
    : m_id { server.next_window_id() }
    , m_size { size }
{
    m_surface = server.compositor().create_surface();

    m_xdg_surface = server.xdg_wm_base().get_xdg_surface(m_surface);
    m_xdg_surface.on_configure() = [this](uint32_t serial) {
        m_xdg_surface.ack_configure(serial);
        draw();
    };

    m_xdg_toplevel = m_xdg_surface.get_toplevel();
    m_xdg_toplevel.set_title("Hello, Wayland!");

    m_surface.commit();

    server.display().roundtrip();

    m_egl_window = egl_window_t { m_surface, static_cast<int>(size.width), static_cast<int>(size.height) };

    m_egl_display = server.egl_display();

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

    m_egl_surface = eglCreatePlatformWindowSurface(m_egl_display, egl_config, m_egl_window, NULL);

    EGLint context_attribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_NONE
    };
    m_egl_context = eglCreateContext(m_egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);

    eglMakeCurrent(m_egl_display, m_egl_surface, m_egl_surface, m_egl_context);

    glViewport(0, 0, m_size.width, m_size.height);

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

    glm::mat4 projection = glm::ortho(0.0f, m_size.width, m_size.height, 0.0f, -1.0f, 1.0f);
    unsigned int projection_loc = glGetUniformLocation(m_program, "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, glm::value_ptr(projection));
}

Window::~Window()
{
    glUseProgram(0);
    glDeleteProgram(m_program);

    if (eglGetCurrentContext() == m_egl_context) {
        eglMakeCurrent(m_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
    eglDestroyContext(m_egl_display, m_egl_context);
    eglDestroySurface(m_egl_display, m_egl_surface);
}

auto Window::draw() -> void
{
    if (m_animate) {
        static GLfloat color[4] = { 0.0, 1.0, 0.0, 1.0 };
        static int inc = 0, dec = 1;

        color[inc] += 0.01;
        color[dec] -= 0.01;

        if (color[dec] <= 0.0) {
            dec = inc;
            inc = (inc + 2) % 3;
        }
    
        glClearColor(color[0], color[1], color[2], color[3]);
    } else {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 transform = glm::mat4(1.0f);
    if (m_animate) {
        static float angle = 0.0f;
        angle += 0.01f;
        transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    unsigned int transform_loc = glGetUniformLocation(m_program, "transform");
    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

    glUseProgram(m_program);

    auto ui_window = ui::Window::with_id(m_id);
    ui_window->layer()->draw();
    ui_window->layer()->composite();

    eglSwapBuffers(m_egl_display, m_egl_surface);

#ifndef NDEBUG
    print_fps();
#endif
}

}
