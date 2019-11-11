#include <NoFlowers/World/Chunk.h>

Chunk::Chunk()
: _dirty(true)
{
    for (size_t i = 0; i < CHUNK_X * CHUNK_Y * 5; ++i)
    {
        _blocks[i] = BlockID::Grass;
    }
    for (size_t i = CHUNK_X * CHUNK_Y * 5; i < CHUNK_BLOCKS; ++i)
    {
        _blocks[i] = BlockID::Air;
    }
}
