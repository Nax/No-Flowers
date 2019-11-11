#include <NoFlowers/World/Block.h>

static Block kBlocks[256];

void Block::init()
{
#define X(id, symbolicId, materialId) kBlocks[id] = { MaterialID::materialId };
#include "Block.inc"
#undef X
}

const Block& Block::fromID(BlockID id)
{
    return kBlocks[unsigned(id)];
}
