#include <Core/EventLoop.h>
#include <Gfx/GLES/Program.h>
#include <Platform/Window.h>
#include <chrono>
#include <cmath>
#include <cstdlib>

using namespace std::chrono;

static const GLchar* vertexShaderSource = R"(
#version 300 es

layout(location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

static const GLchar* fragmentShaderSource = R"(
#version 300 es

precision mediump float;

out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
    FragColor = ourColor;
}
)";

class WindowDelegate final : public Object, public Platform::Window::Delegate
{
public:
    void drawWindow(StrongPtr<Platform::Window> window) override
    {
        ensureProgram();
        ensureVao();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto currentTime = high_resolution_clock::now();
        duration<float> elapsed = currentTime - _startTime;
        float greenValue = (std::sin(elapsed.count()) / 2.0f) + 0.5f;

        _program->use();
        _program->setUniform("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window->setNeedsDraw();
    }

private:
    void ensureProgram()
    {
        if (!_program)
            _program = Gfx::GLES::Program::program(vertexShaderSource, fragmentShaderSource);
    }

    void ensureVao()
    {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    StrongPtr<Gfx::GLES::Program> _program = nullptr;
    GLuint _vao = 0;
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
