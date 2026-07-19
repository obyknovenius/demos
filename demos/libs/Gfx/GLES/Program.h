#pragma once

#include <Foundation/Object.h>
#include <Foundation/StrongPtr.h>
#include <GLES3/gl3.h>

namespace Gfx::GLES
{
    class Program final : public Object
    {
    public:
        static StrongPtr<Program> program(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource)
        {
            return StrongPtr<Program>::adopt(new Program(vertexShaderSource, fragmentShaderSource));
        }

        void use() const
        {
            glUseProgram(_program);
        }

        void setUniform(const GLchar* name, GLint value0) const
        {
            glUniform1i(glGetUniformLocation(_program, name), value0);
        }

        void setUniform(const GLchar* name, GLint value0, GLint value1) const
        {
            glUniform2i(glGetUniformLocation(_program, name), value0, value1);
        }

        void setUniform(const GLchar* name, GLint value0, GLint value1, GLint value2) const
        {
            glUniform3i(glGetUniformLocation(_program, name), value0, value1, value2);
        }

        void setUniform(const GLchar* name, GLint value0, GLint value1, GLint value2, GLint value3) const
        {
            glUniform4i(glGetUniformLocation(_program, name), value0, value1, value2, value3);
        }

        void setUniform(const GLchar* name, GLfloat value0) const
        {
            glUniform1f(glGetUniformLocation(_program, name), value0);
        }

        void setUniform(const GLchar* name, GLfloat value0, GLfloat value1) const
        {
            glUniform2f(glGetUniformLocation(_program, name), value0, value1);
        }

        void setUniform(const GLchar* name, GLfloat value0, GLfloat value1, GLfloat value2) const
        {
            glUniform3f(glGetUniformLocation(_program, name), value0, value1, value2);
        }

        void setUniform(const GLchar* name, GLfloat value0, GLfloat value1, GLfloat value2, GLfloat value3) const
        {
            glUniform4f(glGetUniformLocation(_program, name), value0, value1, value2, value3);
        }

        void setUniform(const GLchar* name, const GLfloat* value) const
        {
            glUniformMatrix4fv(glGetUniformLocation(_program, name), 1, GL_FALSE, value);
        }

    private:
        Program(const GLchar* vertexShaderSource, const GLchar* fragmentShaderSource);

        ~Program() override
        {
            if (_program)
                glDeleteProgram(_program);
        }

        GLuint _program;
    };
}
