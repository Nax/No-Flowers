#ifndef RENDER_RENDERER_H
#define RENDER_RENDERER_H

#include <NoFlowers/Util/NonCopyable.h>
#include <NoFlowers/Render/Shader.h>

class Camera;
class Chunk;
class Renderer : public NonCopyable
{
public:
    Renderer();
    ~Renderer();

    void render(const Chunk& chunk, const Camera& camera);

private:
    void    _initShaders();
    void    _prepareChunk(const Chunk& chunk);

    Shader  _shader;
};

#endif
