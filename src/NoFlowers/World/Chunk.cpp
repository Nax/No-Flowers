#include <NoFlowers/World/Chunk.h>

Chunk::Chunk()
: _dirty(true)
{
    for (size_t i = 0; i < CHUNK_BLOCKS; ++i)
    {
        _blocks[i] = BlockID::Air;
    }

    for (int i = 0; i < 6; ++i)
        memset(_blockAdjacent[i], 0, sizeof(_blockAdjacent[i]));

    for (int i = 0; i < CHUNK_X * CHUNK_Y; ++i)
    {
        setBlock(i, BlockID::Bedrock);
    }
    for (int i = CHUNK_X * CHUNK_Y; i < CHUNK_X * CHUNK_Y * 10; ++i)
    {
        setBlock(i, BlockID::Rock);
    }
    for (int i = CHUNK_X * CHUNK_Y * 10; i < CHUNK_X * CHUNK_Y * 14; ++i)
    {
        setBlock(i, BlockID::Dirt);
    }
    for (int i = CHUNK_X * CHUNK_Y * 14; i < CHUNK_X * CHUNK_Y * 15; ++i)
    {
        setBlock(i, BlockID::Grass);
    }
}

Chunk::~Chunk()
{

}

bool Chunk::isAdjacent(unsigned linear, int dir) const
{
    unsigned bit = linear % 8;
    return (_blockAdjacent[dir][linear / 8] & (1 << bit));
}

void Chunk::setBlock(unsigned linear, BlockID block)
{
    bool air;

    unsigned x;
    unsigned y;
    unsigned z;

    air = !(bool)block;

    x = linear % CHUNK_X;
    y = (linear / CHUNK_X) % CHUNK_Y;
    z = linear / (CHUNK_X * CHUNK_Y);

    _blocks[linear] = block;

    if (air)
    {
        if (x + 1 < CHUNK_X) _setAdjacent(linear + 1, 0);
        if (y + 1 < CHUNK_Y) _setAdjacent(linear + CHUNK_X, 1);
        if (z + 1 < CHUNK_Z) _setAdjacent(linear + CHUNK_X * CHUNK_Y, 2);
        if (x > 0) _setAdjacent(linear - 1, 3);
        if (y > 0) _setAdjacent(linear - CHUNK_X, 4);
        if (z > 0) _setAdjacent(linear - CHUNK_X * CHUNK_Y, 5);
    }
    else
    {
        if (x + 1 < CHUNK_X)
            _tryAdjacent(linear, 1, 0, 3);
        else
            _setAdjacent(linear, 0);

        if (y + 1 < CHUNK_Y)
            _tryAdjacent(linear, CHUNK_X, 1, 4);
        else
            _setAdjacent(linear, 1);

        if (z + 1 < CHUNK_Z)
            _tryAdjacent(linear, CHUNK_X * CHUNK_Y, 2, 5);
        else
            _setAdjacent(linear, 2);

        if (x > 0)
            _tryAdjacent(linear, -1, 3, 0);
        else
            _setAdjacent(linear, 3);

        if (y > 0)
            _tryAdjacent(linear, -CHUNK_X, 4, 1);
        else
            _setAdjacent(linear, 4);

        if (z > 0)
            _tryAdjacent(linear, -(CHUNK_X * CHUNK_Y), 5, 2);
        else
            _setAdjacent(linear, 5);
    }

    _dirty = true;
}

void Chunk::_setAdjacent(unsigned linear, int dir)
{
    unsigned bit = linear % 8;
    _blockAdjacent[dir][linear / 8] |= (1 << bit);
}

void Chunk::_clearAdjacent(unsigned linear, int dir)
{
    unsigned bit = linear % 8;
    _blockAdjacent[dir][linear / 8] &= ~(1 << bit);
}

void Chunk::_tryAdjacent(unsigned linear, int delta, int dir, int revDir)
{
    bool otherAir;

    otherAir = !(bool)_blocks[linear + delta];
    if (otherAir)
    {
        _setAdjacent(linear, dir);
    }
    else
    {
        _clearAdjacent(linear, dir);
        _clearAdjacent(linear + delta, revDir);
    }
}
