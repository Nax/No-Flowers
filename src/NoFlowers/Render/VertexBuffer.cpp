#include <algorithm>
#include <NoFlowers/Render/VertexBuffer.h>

VertexBuffer::VertexBuffer()
: _vao(0)
, _vbo(0)
, _ibo(0)
, _size(0)
{

}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
: _vao(other._vao)
, _vbo(other._vbo)
, _ibo(other._ibo)
{
    other._vao = 0;
    other._vbo = 0;
    other._ibo = 0;
}

VertexBuffer::~VertexBuffer()
{
    if (_vao)
        glDeleteVertexArrays(1, &_vao);
    if (_vbo)
        glDeleteBuffers(1, &_vbo);
    if (_ibo)
        glDeleteBuffers(1, &_ibo);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
{
    std::swap(_vao, other._vao);
    std::swap(_vbo, other._vbo);
    std::swap(_ibo, other._ibo);

    return *this;
}

size_t VertexBuffer::size() const
{
    return _size;
}

void VertexBuffer::bind() const
{
    glBindVertexArray(_vao);
}
