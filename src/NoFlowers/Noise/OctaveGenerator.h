#ifndef OCTAVE_GENERATOR_H
#define OCTABE_GENERATOR_H

#include <cstdint>
#include <NoFlowers/Math/Vector.h>
#include <NoFlowers/Util/NonCopyable.h>

template <typename Generator, size_t N>
class OctaveGenerator : private NonCopyable
{
public:
    float at(Vector2f pos)
    {
        float acc;
        float accAmplitude;
        float power;
        float step;

        acc = 0.f;
        power = 1.f;
        step = 1.f;
        accAmplitude = 0.f;

        for (size_t i = 0; i < N; ++i)
        {
            acc += power * _generators[i].at(pos * step);
            accAmplitude += power;
            power *= 0.5;
            step *= 2.f;
        }

        return acc / accAmplitude;
    }

private:
    Generator _generators[N];
};

#endif
