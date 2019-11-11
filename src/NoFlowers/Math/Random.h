#ifndef RANDOM_H
#define RANDOM_H 1

#include <cstring>
#include <cstdlib>
#include <cstdint>

inline static uint64_t rol64(uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

class Random
{
public:
    Random()
    {
        memset(_state, 0, sizeof(_state));
    }

    void seed(const void* src)
    {
        memcpy(_state, src, sizeof(_state));
    }

    void seed(Random& other)
    {
        for (int i = 0; i < 4; ++i)
            _state[i] = other.rand();
    }

    void seed()
    {
        uint32_t tmp[8];

        for (int i = 0; i < 8; ++i)
            rand_s(tmp + i);
        seed(tmp);
    }

    uint64_t rand()
    {
        uint64_t result;
        uint64_t t;

        result = rol64(_state[1] * 5, 7) * 9;
        t = _state[1] << 17;

        _state[2] ^= _state[0];
        _state[3] ^= _state[1];
        _state[1] ^= _state[2];
        _state[0] ^= _state[3];

        _state[2] ^= t;
        _state[3] = rol64(_state[3], 45);

        return result;
    }

private:
    uint64_t _state[4];
};

#endif
