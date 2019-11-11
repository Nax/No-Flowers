#include <NoFlowers/World/Material.h>

static Material kMaterials[256];

void Material::init()
{
#define X(id, symbolicId, color) kMaterials[id] = { color };
#include "Material.inc"
#undef X
}

const Material& Material::fromID(MaterialID id)
{
    return kMaterials[unsigned(id)];
}
