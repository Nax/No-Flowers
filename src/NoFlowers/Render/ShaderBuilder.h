#ifndef RENDER_SHADER_BUILDER_H
#define RENDER_SHADER_BUILDER_H 1

#include <vector>
#include <NoFlowers/Render/Shader.h>

enum class ShaderType
{
    Vertex,
    Fragment
};

class ShaderBuilder
{
public:
    ShaderBuilder();
    ~ShaderBuilder();

    void addSource(ShaderType type, const char* str);
    void addSource(ShaderType type, const char* str, size_t size);

    void bindAttribLocation(const char* attrib, int loc);

    Shader link();

private:
    void                _initProgram();
    void                _reset();

    std::vector<GLuint> _shaders;
    GLuint              _program;
};

#endif
