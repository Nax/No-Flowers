#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H 1

#include <cstdint>
#include <NoFlowers/Util/NonCopyable.h>
#include <NoFlowers/Noise/PerlinNoiseGenerator.h>
#include <NoFlowers/Noise/OctaveGenerator.h>

class World;
class WorldGenerator : private NonCopyable
{
public:
    WorldGenerator(World& world);
    ~WorldGenerator();

    void generate();

private:
    unsigned heightAt(int32_t x, int32_t y);

    World&                                      _world;
    OctaveGenerator<PerlinNoiseGenerator, 4>    _noiseGenerator;
};

#endif
