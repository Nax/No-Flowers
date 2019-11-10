#include <cstdlib>
#include <NoFlowers/Render/ShaderBuilder.h>
#include <NoFlowers/Util/Log.h>

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
    GLint success;
    GLint length;
    GLuint shader;
    GLenum shaderType;

    success = 0;
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
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint logSize;
        char* log;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
        log = new char[logSize + 1];
        memset(log, 0, logSize + 1);
        glGetShaderInfoLog(shader, logSize, NULL, log);
        Log::print("%s", log);
        delete[] log;
        glDeleteShader(shader);
    }
    else
    {
        glAttachShader(_program, shader);
        _shaders.push_back(shader);
    }
}

void ShaderBuilder::bindAttribLocation(const char* attrib, int loc)
{
    _initProgram();
    glBindAttribLocation(_program, loc, attrib);
}

Shader ShaderBuilder::link()
{
    GLint success;
    Shader shader;

    _initProgram();
    success = 0;
    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint logSize;
        char* log;

        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &logSize);
        log = new char[logSize + 1];
        memset(log, 0, logSize + 1);
        glGetProgramInfoLog(_program, logSize, NULL, log);
        Log::print("%s", log);
        delete[] log;
    }
    else
    {
        shader._program = _program;
        _program = 0;
    }
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
