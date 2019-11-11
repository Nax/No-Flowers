#ifndef NOISE_PERLIN_NOISE_GENERATOR_H
#define NOISE_PERLIN_NOISE_GENERATOR_H 1

#include <cstdint>
#include <NoFlowers/Math/Vector.h>
#include <NoFlowers/Util/NonCopyable.h>

#define PERLIN_GRADIENTS    1024

class PerlinNoiseGenerator : private NonCopyable
{
public:
    PerlinNoiseGenerator();

    float at(Vector2f pos) const;

private:
    uint64_t    _seed;
    Vector2f    _gradients[PERLIN_GRADIENTS];
};

#endif
