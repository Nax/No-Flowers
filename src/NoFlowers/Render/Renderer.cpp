#include <NoFlowers/Render/Renderer.h>
#include <NoFlowers/Render/ShaderBuilder.h>
#include <NoFlowers/Render/VertexBufferBuilder.h>
#include <NoFlowers/World/Chunk.h>
#include <NoFlowers/Interface/Camera.h>

static const char* kShaderVertex = R"(
#version 150

in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;

uniform mat4 mvp;

out vec4 fColor;

void main()
{
    vec3 lightDir = normalize(vec3(1.0, 2.0, -10.0));
    float light = mix(0.6, 1.0, max(dot(lightDir, -vNormal), 0.0));
    fColor = vColor * light;
    gl_Position = vec4(vPosition, 1.0) * mvp;
}
)";

static const char* kShaderFragment = R"(
#version 150

in vec4 fColor;

out vec4 outColor;

void main()
{
    outColor = fColor;
}
)";

Renderer::Renderer()
{
    _initShaders();
}

Renderer::~Renderer()
{

}

void Renderer::render(const Chunk& chunk, const Camera& camera)
{
    Matrix4f model;
    Matrix4f view;
    Matrix4f projection;
    Matrix4f mvp;

    model = Matrix4f::identity();
    view = camera.viewMatrix();
    projection = camera.projectionMatrix();

    mvp = projection * view * model;

    if (chunk._dirty)
    {
        _prepareChunk(chunk);
        chunk._dirty = false;
    }

    _shader.bind();
    _shader.uniform(ShaderUniform::MVP, mvp);
    chunk._vertexBuffer.bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)chunk._vertexBuffer.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::_initShaders()
{
    ShaderBuilder builder;

    builder.addSource(ShaderType::Vertex, kShaderVertex);
    builder.addSource(ShaderType::Fragment, kShaderFragment);

    builder.bindAttribLocation("vPosition", 0);
    builder.bindAttribLocation("vNormal", 1);
    builder.bindAttribLocation("vColor", 2);

    _shader = builder.link();
}

static void makeQuad(VertexBufferBuilder& builder, Vector3f normal, Vector4f color, Vector3f base, Vector3f d1, Vector3f d2)
{
    builder.push(base);
    builder.push(normal);
    builder.push(color);
    builder.push(base + d1);
    builder.push(normal);
    builder.push(color);
    builder.push(base + d1 + d2);
    builder.push(normal);
    builder.push(color);
    builder.push(base + d2);
    builder.push(normal);
    builder.push(color);
    builder.makeQuad();
}

static void makeCube(VertexBufferBuilder& builder, const Chunk& chunk, unsigned linear, Vector4f color, Vector3f pos)
{
    if (chunk.isAdjacent(linear, DIR_X_POS))
        makeQuad(builder, Vector3f(1.f, 0.f, 0.f), color, pos + Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f), Vector3f(0.f, 0.f, 1.f));
    if (chunk.isAdjacent(linear, DIR_Y_POS))
        makeQuad(builder, Vector3f(0.f, 1.f, 0.f), color, pos + Vector3f(0.f, 1.f, 0.f), Vector3f(0.f, 0.f, 1.f), Vector3f(1.f, 0.f, 0.f));
    if (chunk.isAdjacent(linear, DIR_Z_POS))
        makeQuad(builder, Vector3f(0.f, 0.f, 1.f), color, pos + Vector3f(0.f, 0.f, 1.f), Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f));
    if (chunk.isAdjacent(linear, DIR_X_NEG))
        makeQuad(builder, Vector3f(-1.f, 0.f, 0.f), color, pos, Vector3f(0.f, 0.f, 1.f), Vector3f(0.f, 1.f, 0.f));
    if (chunk.isAdjacent(linear, DIR_Y_NEG))
        makeQuad(builder, Vector3f(0.f, -1.f, 0.f), color, pos, Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 0.f, 1.f));
    if (chunk.isAdjacent(linear, DIR_Z_NEG))
        makeQuad(builder, Vector3f(0.f, 0.f, -1.f), color, pos, Vector3f(0.f, 1.f, 0.f), Vector3f(1.f, 0.f, 0.f));
}

void Renderer::_prepareChunk(const Chunk& chunk)
{
    VertexBufferBuilder builder(chunk._vertexBuffer);
    BlockID blockID;
    unsigned x;
    unsigned y;
    unsigned z;

    builder.attr(0, 3);
    builder.attr(1, 3);
    builder.attr(2, 4);

    for (unsigned i = 0; i < CHUNK_BLOCKS; ++i)
    {
        x = i % CHUNK_X;
        y = (i / CHUNK_X) % CHUNK_Y;
        z = (i / (CHUNK_X * CHUNK_Y));

        blockID = chunk._blocks[i];
        if ((unsigned)blockID)
        {
            const Material& mat = Material::fromID(Block::fromID(blockID).materialId);
            makeCube(builder, chunk, i, Vector4f(Vector3f(mat.color) / 255.f, 1.f), Vector3f(x, y, z));
        }
    }

    builder.submit();
}
