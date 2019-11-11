#include <cstdint>
#include <NoFlowers/Noise/PerlinNoiseGenerator.h>
#include <NoFlowers/Math/Constants.h>

static Vector2f randomGradient(Random& r)
{
    float f;

    f = ((r.rand() % 65536) / 65536.f) * Math::tau;
    return Vector2f(cosf(f), sinf(f));
}

static uint32_t coordinateHash(uint32_t x, uint32_t y)
{
    return ((((x ^ (x >> 16)) * 0x55555555) ^ y) * 0x187ab541) ^ y;
}

static float smooth(float t)
{
    return (6 * (t * t * t * t * t) - 15 * ((t * t) * (t * t)) + 10 * (t * t * t));
}

static float lerp(float a, float b, float w)
{
    return (1 - w) * a + w * b;
}

PerlinNoiseGenerator::PerlinNoiseGenerator()
{

}

void PerlinNoiseGenerator::seed(Random& r)
{
    for (int i = 0; i < PERLIN_GRADIENTS; ++i)
        _gradients[i] = randomGradient(r);
}

#define SQRT2 (1.41421356237f)

float PerlinNoiseGenerator::at(Vector2f pos) const
{
    float g[4];

    int x0;
    int x1;
    int y0;
    int y1;

    float ax;
    float ay;

    float s1;
    float s2;
    float v;

    x0 = (int)floorf(pos.x);
    y0 = (int)floorf(pos.y);
    x1 = x0 + 1;
    y1 = y0 + 1;

    ax = smooth(pos.x - floorf(pos.x));
    ay = smooth(pos.y - floorf(pos.y));

    g[0] = dot(_gradients[coordinateHash(x0, y0) % PERLIN_GRADIENTS], Vector2f(ax, ay));
    g[1] = dot(_gradients[coordinateHash(x1, y0) % PERLIN_GRADIENTS], Vector2f(1.f - ax, ay));
    g[2] = dot(_gradients[coordinateHash(x0, y1) % PERLIN_GRADIENTS], Vector2f(ax, 1.f - ay));
    g[3] = dot(_gradients[coordinateHash(x1, y1) % PERLIN_GRADIENTS], Vector2f(1.f - ax, 1.f - ay));

    s1 = lerp(g[0], g[1], ax);
    s2 = lerp(g[2], g[3], ax);
    v = lerp(s1, s2, ay);

    return ((v / SQRT2) + 1.f) * 0.5f;
}
