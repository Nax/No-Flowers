#include <algorithm>
#include <NoFlowers/Render/Shader.h>

Shader::Shader()
: _program(0)
{
    memset(_locations, 0, sizeof(_locations));
}

Shader::Shader(Shader&& other)
: _program(other._program)
{
    memcpy(_locations, other._locations, sizeof(_locations));
    other._program = 0;
}

Shader::~Shader()
{
    if (_program)
    {
        glDeleteProgram(_program);
    }
}

Shader& Shader::operator=(Shader&& other)
{
    std::swap(_program, other._program);
    memcpy(_locations, other._locations, sizeof(_locations));
    return *this;
}

void Shader::bind() const
{
    glUseProgram(_program);
}

void Shader::uniform(ShaderUniform uniform, Matrix4f mat)
{
    glUniformMatrix4fv(_locations[int(uniform)], 1, GL_FALSE, mat.data());
}
