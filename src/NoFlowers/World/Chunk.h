#ifndef CHUNK_H
#define CHUNK_H 1

#include <NoFlowers/Renderer/VertexBuffer.h>
#include <NoFlowers/World/Block.h>

#define CHUNK_X 16
#define CHUNK_Y 16
#define CHUNK_Z 128

class Chunk
{
public:
    Chunk();
    ~Chunk();

    void render();

private:
    void _prepare();

    bool            _dirty;
    VertexBuffer    _vertexBuffer;
    BlockID         _blocks[CHUNK_X * CHUNK_Y * CHUNK_Z];
};

#endif
