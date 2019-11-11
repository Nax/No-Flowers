#include <NoFlowers/World/World.h>

World::World()
{
    for (unsigned i = 0; i < WORLD_X * WORLD_Y; ++i)
    {
        _chunks[i] = new Chunk();
    }
}

World::~World()
{
    for (unsigned i = 0; i < WORLD_X * WORLD_Y; ++i)
    {
        delete _chunks[i];
    }
}
