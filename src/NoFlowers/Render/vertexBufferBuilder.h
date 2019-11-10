#ifndef RENDER_VERETX_BUFFER_BUILDER_H
#define RENDER_VERTEX_BUFFER_BUILDER_H 1

#include <cstdint>
#include <vector>
#include <NoFlowers/Math/Vector.h>
#include <NoFlowers/Util/NonCopyable.h>
#include <NoFlowers/Render/VertexBuffer.h>

class VertexBufferBuilder : private NonCopyable
{
public:
    VertexBufferBuilder(VertexBuffer& vb);
    ~VertexBufferBuilder();

    void attr(int num, int dim);

    void push(Vector2f v);
    void push(Vector3f v);
    void push(Vector4f v);

    void makeTriangle();
    void makeQuad();

    void submit();

private:
    struct Attr
    {
        int num;
        int dim;
    };

    VertexBuffer&           _vb;
    uint32_t                _index;
    size_t                  _size;
    std::vector<Attr>       _attr;
    std::vector<float>      _data;
    std::vector<uint32_t>   _indices;
};

#endif
