#include <algorithm>
#include <NoFlowers/Render/Shader.h>

Shader::Shader()
: _program(0)
{

}

Shader::Shader(Shader&& other)
: _program(other._program)
{
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
    return *this;
}

void Shader::bind() const
{
    glUseProgram(_program);
}
