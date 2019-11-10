#include <cstdlib>
#include <NoFlowers/Render/ShaderBuilder.h>

ShaderBuilder::ShaderBuilder()
: _program(0)
{

}

ShaderBuilder::~ShaderBuilder()
{
    _reset();
}

void ShaderBuilder::addSource(ShaderType type, const char* str)
{
    addSource(type, str, strlen(str));
}

void ShaderBuilder::addSource(ShaderType type, const char* str, size_t size)
{
    GLint length;
    GLuint shader;
    GLenum shaderType;

    _initProgram();

    switch (type)
    {
    case ShaderType::Vertex:
        shaderType = GL_VERTEX_SHADER;
        break;
    case ShaderType::Fragment:
        shaderType = GL_FRAGMENT_SHADER;
        break;
    }

    length = (GLsizei)size;
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &str, &length);
    glCompileShader(shader);
    glAttachShader(_program, shader);
    _shaders.push_back(shader);
}

void ShaderBuilder::bindAttribLocation(const char* attrib, int loc)
{
    _initProgram();
    glBindAttribLocation(_program, loc, attrib);
}

Shader ShaderBuilder::link()
{
    Shader shader;

    _initProgram();
    glLinkProgram(_program);
    shader._program = _program;
    _reset();

    return shader;
}

void ShaderBuilder::_initProgram()
{
    if (!_program)
    {
        _program = glCreateProgram();
    }
}

void ShaderBuilder::_reset()
{
    for (auto s : _shaders)
    {
        glDetachShader(_program, s);
        glDeleteShader(s);
    }

    if (_program)
    {
        glDeleteProgram(_program);
    }

    _shaders.clear();
    _program = 0;
}
