#include "config.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <Core/EventLoop.h>
#include <Gfx/GLES/Program.h>
#include <Platform/Window.h>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <iostream>

using namespace std::chrono;

std::filesystem::path dataPath()
{
    if (const char* dataPath = std::getenv("PKGDATADIR"))
        return dataPath;

    return PKGDATADIR;
}

static const GLchar* vertexShaderSource = R"(
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

static const GLchar* fragmentShaderSource = R"(
#version 300 es

precision mediump float;

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}
)";

class WindowDelegate final : public Object, public Platform::Window::Delegate
{
public:
    ~WindowDelegate() override
    {
        if (_vao)
            glDeleteVertexArrays(1, &_vao);

        if (_texture)
            glDeleteTextures(1, &_texture);
    }

    void drawWindow(StrongPtr<Platform::Window> window) override
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto currentTime = high_resolution_clock::now();
        duration<float> elapsed = currentTime - _startTime;
        float greenValue = (std::sin(elapsed.count()) / 2.0f) + 0.5f;

        ensureProgram();

        _program->use();
        _program->setUniform("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

        ensureTexture();

        glBindTexture(GL_TEXTURE_2D, _texture);

        ensureVao();

        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        window->setNeedsDraw();
    }

private:
    void ensureProgram()
    {
        if (_program)
            return;

        _program = Gfx::GLES::Program::program(vertexShaderSource, fragmentShaderSource);
    }

    void ensureTexture()
    {
        if (_texture)
            return;

        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        auto texturePath = dataPath() / "container.jpg";
        int width, height, channels;
        GLubyte* data = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
        stbi_image_free(data);
    }

    void ensureVao()
    {
        if (_vao)
            return;

        GLfloat vertices[] = {
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // top left
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f   // bottom right
        };

        GLuint indices[] = {
            0, 1, 2, // first triangle
            1, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    StrongPtr<Gfx::GLES::Program> _program = nullptr;

    GLuint _vao = 0;
    GLuint _texture = 0;

    time_point<high_resolution_clock> _startTime = high_resolution_clock::now();
};

int main(int argc, char** argv)
{
    auto window = Platform::Window::window();

    auto windowDelegate = makeStrong<WindowDelegate>();
    window->setDelegate(windowDelegate);

    Core::EventLoop::mainLoop()->run();

    return 0;
}
