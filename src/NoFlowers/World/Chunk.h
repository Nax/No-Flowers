#ifndef CHUNK_H
#define CHUNK_H 1

#include <NoFlowers/Render/VertexBuffer.h>
#include <NoFlowers/World/Block.h>

#define CHUNK_X         16
#define CHUNK_Y         16
#define CHUNK_Z         128
#define CHUNK_BLOCKS    (CHUNK_X * CHUNK_Y * CHUNK_Z)

class Renderer;
class Chunk
{
    friend class Renderer;
public:
    Chunk();
    ~Chunk();

private:
    mutable bool            _dirty;
    mutable VertexBuffer    _vertexBuffer;
    BlockID                 _blocks[CHUNK_BLOCKS];
};

#endif
