#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H 1

#include <cstdint>
#include <NoFlowers/Util/NonCopyable.h>
#include <NoFlowers/Render/OpenGL.h>
#include <NoFlowers/Math/Matrix.h>

class ShaderBuilder;

enum class ShaderUniform : uint8_t
{
    MVP = 0
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
    void uniform(ShaderUniform uniform, Matrix4f mat);

private:
    GLuint  _program;
    GLuint  _locations[32];
};

#endif
