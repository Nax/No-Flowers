#ifndef WORLD_H
#define WORLD_H 1

#include <NoFlowers/Util/NonCopyable.h>
#include <NoFlowers/World/Chunk.h>

#define WORLD_X 10
#define WORLD_Y 10

class Renderer;
class WorldGenerator;
class World : private NonCopyable
{
    friend class Renderer;
    friend class WorldGenerator;

public:
    World();
    ~World();

private:
    Chunk* _chunks[WORLD_X * WORLD_Y];
};

#endif
