#ifndef BLOCK_H
#define BLOCK_H 1

#include <cstdint>
#include <NoFlowers/World/Material.h>

enum class BlockID : uint8_t
{
#define X(id, symbolicId, _)   symbolicId = id,
#include "Block.inc"
#undef X
};

struct Block
{
public:
    MaterialID materialId;

    static void init();
    static const Block& fromId(BlockID id);
};

#endif
