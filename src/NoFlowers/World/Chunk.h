#ifndef CHUNK_H
#define CHUNK_H 1

#include <NoFlowers/Render/VertexBuffer.h>
#include <NoFlowers/World/Block.h>
#include <NoFlowers/World/Direction.h>

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

    bool isAdjacent(unsigned linear, int dir) const;

    void setBlock(unsigned linear, BlockID block);

private:
    void _setAdjacent(unsigned linear, int dir);
    void _clearAdjacent(unsigned linear, int dir);
    void _tryAdjacent(unsigned linear, int delta, int dir, int revDir);

    mutable bool            _dirty;
    mutable VertexBuffer    _vertexBuffer;
    BlockID                 _blocks[CHUNK_BLOCKS];
    uint8_t                 _blockAdjacent[6][CHUNK_BLOCKS / 8];
};

#endif
