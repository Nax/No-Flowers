#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H 1

#include <cstdint>
#include <NoFlowers/Util/NonCopyable.h>
#include <NoFlowers/Render/OpenGL.h>

class ShaderBuilder;

enum class ShaderUniform : uint8_t
{
};

class Shader : private NonCopyable
{
    friend class ShaderBuilder;

public:
    Shader();
    Shader(Shader&& other);
    ~Shader();

    Shader& operator=(Shader&& other);

    void bind() const;

private:
    GLuint _program;
};

#endif
