#ifndef MATERIAL_H
#define MATERIAL_H 1

#include <cstdint>
#include <NoFlowers/Math/Vector.h>

enum class MaterialID : uint8_t
{
#define X(id, symbolicId, color) symbolicId = id,
#include "Material.inc"
#undef X
};

struct Material
{
public:
    Vector3b color;

    static void init();
    static const Material& fromID(MaterialID id);
};

#endif
