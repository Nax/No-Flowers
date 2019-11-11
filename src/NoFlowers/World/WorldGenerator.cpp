#include <NoFlowers/World/WorldGenerator.h>
#include <NoFlowers/World/World.h>

WorldGenerator::WorldGenerator(World& world)
: _world(world)
{
    Random random;

    random.seed();
    _noiseGenerator.seed(random);
}

WorldGenerator::~WorldGenerator()
{

}

void WorldGenerator::generate()
{
    unsigned cx;
    unsigned cy;
    unsigned x;
    unsigned y;
    unsigned h;

    for (unsigned i = 0; i < WORLD_X * WORLD_Y; ++i)
    {
        Chunk& chunk = *_world._chunks[i];
        cx = i % WORLD_X;
        cy = i / WORLD_X;

        for (unsigned j = 0; j < CHUNK_X * CHUNK_Y; ++j)
        {
            x = j % CHUNK_X;
            y = j / CHUNK_X;
            h = heightAt(cx * CHUNK_X + x, cy * CHUNK_Y + y);
            for (unsigned z = 0; z < h; ++z)
            {
                chunk.setBlock(x, y, z, BlockID::Grass);
            }
        }
    }
}

unsigned WorldGenerator::heightAt(int32_t x, int32_t y)
{
    float scale = 64;
    float noise;

    noise = _noiseGenerator.at(Vector2f(x / scale, y / scale));

    return 20 + 100 * noise;
}
