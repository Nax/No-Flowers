#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <NoFlowers/Util/NonCopyable.h>
#include <NoFlowers/Render/Shader.h>

class Camera;
class Chunk;
class World;
class Renderer : public NonCopyable
{
public:
    Renderer();
    ~Renderer();

    void render(const World& world, const Camera& camera);

private:
    void    _initShaders();
    void    _initShadows();

    void    _prepareChunk(const Chunk& chunk);
    void    _render(const World& world, const Matrix4f& viewProj, const Shader& shader);
    void    _render(const Chunk& chunk, int cx, int cy, const Matrix4f& viewProj, const Shader& shader);

    GLuint  _shadowTexture;
    GLuint  _shadowFramebuffer;
    Shader  _shadowShader;

    Shader  _shader;
};

#endif
