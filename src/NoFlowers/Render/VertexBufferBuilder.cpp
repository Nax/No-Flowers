#include <NoFlowers/Render/VertexBufferBuilder.h>

VertexBufferBuilder::VertexBufferBuilder(VertexBuffer& vb)
: _vb(vb)
, _index(0)
, _size(0)
{
    if (!vb._vao)
        glCreateVertexArrays(1, &vb._vao);
    if (!vb._vbo)
        glCreateBuffers(1, &vb._vbo);
    if (!vb._ibo)
        glCreateBuffers(1, &vb._ibo);
}

VertexBufferBuilder::~VertexBufferBuilder()
{

}

void VertexBufferBuilder::attr(int num, int dim)
{
    _attr.push_back({ num, dim });
}

void VertexBufferBuilder::push(Vector2f v)
{
    _data.push_back(v[0]);
    _data.push_back(v[1]);
}

void VertexBufferBuilder::push(Vector3f v)
{
    _data.push_back(v[0]);
    _data.push_back(v[1]);
    _data.push_back(v[2]);
}

void VertexBufferBuilder::push(Vector4f v)
{
    _data.push_back(v[0]);
    _data.push_back(v[1]);
    _data.push_back(v[2]);
    _data.push_back(v[3]);
}

void VertexBufferBuilder::makeTriangle()
{
    _indices.push_back(_index);
    _indices.push_back(_index + 1);
    _indices.push_back(_index + 2);

    _index += 3;
    _index += 3;
}

void VertexBufferBuilder::makeQuad()
{
    _indices.push_back(_index);
    _indices.push_back(_index + 1);
    _indices.push_back(_index + 2);
    _indices.push_back(_index);
    _indices.push_back(_index + 2);
    _indices.push_back(_index + 3);

    _index += 4;
    _size += 6;
}

void VertexBufferBuilder::submit()
{
    GLint dimSize;
    GLint acc;

    dimSize = 0;
    acc = 0;

    glBindVertexArray(_vb._vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vb._vbo);

    glBufferData(GL_ARRAY_BUFFER, _data.size() * sizeof(float), _data.data(), GL_DYNAMIC_DRAW);

    for (auto& a : _attr)
    {
        dimSize += a.dim;
    }

    for (auto& a : _attr)
    {
        glEnableVertexAttribArray(a.num);
        glVertexAttribPointer(a.num, a.dim, GL_FLOAT, GL_FALSE, dimSize * sizeof(float), (const void*)(acc * sizeof(float)));
        acc += a.dim;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vb._ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * 4, _indices.data(), GL_DYNAMIC_DRAW);

    _vb._size = _size;
}
