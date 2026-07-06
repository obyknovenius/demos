#include <Core/EventLoop.h>
#include <GLES3/gl3.h>
#include <Platform/Window.h>
#include <cstdlib>

class WindowDelegate final : public Object, public Platform::Window::Delegate
{
public:
    void drawWindow(StrongPtr<Platform::Window> window) override
    {
        if (!_program)
        {
            unsigned vertexShader = glCreateShader(GL_VERTEX_SHADER);
            const char* vertexShaderSource =
                "#version 300 es\n"
                "layout(location = 0) in vec3 aPos;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = vec4(aPos, 1.0);\n"
                "}\n";

            glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
            glCompileShader(vertexShader);

            unsigned fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            const char* fragmentShaderSource =
                "#version 300 es\n"
                "precision mediump float;\n"
                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
                "}\n";
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
            glCompileShader(fragmentShader);

            _program = glCreateProgram();
            glAttachShader(_program, vertexShader);
            glAttachShader(_program, fragmentShader);
            glLinkProgram(_program);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        if (!_vao)
        {
            float vertices[] = {
                -0.5f, -0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                0.0f, 0.5f, 0.0f
            };

            glGenVertexArrays(1, &_vao);
            glBindVertexArray(_vao);

            unsigned vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(_program);
        glBindVertexArray(_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void windowDidReceiveEvent(StrongPtr<Platform::Window> window, Platform::Event event) override
    {
        window->setNeedsDraw();
    }

private:
    unsigned _vao = 0;
    unsigned _program = 0;
};

int main(int argc, char** argv)
{
    auto window = Platform::Window::window();

    auto windowDelegate = makeStrong<WindowDelegate>();
    window->setDelegate(windowDelegate);

    Core::EventLoop::mainLoop()->run();

    return 0;
}
