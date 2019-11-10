#ifndef RENDER_VERTEX_BUFFER_H
#define RENDER_VERTEX_BUFFER_H 1

#include <cstdint>
#include <NoFlowers/Util/NonCopyable.h>
#include <NoFlowers/Render/OpenGL.h>

class VertexBufferBuilder;

class VertexBuffer : private NonCopyable
{
    friend class VertexBufferBuilder;

public:
    VertexBuffer();
    VertexBuffer(VertexBuffer&& other);
    ~VertexBuffer();

    VertexBuffer& operator=(VertexBuffer&& other);

    size_t  size() const;
    void    bind() const;

private:
    GLuint      _vao;
    GLuint      _vbo;
    GLuint      _ibo;
    size_t      _size;
};

#endif
