#include <NoFlowers/Render/Renderer.h>
#include <NoFlowers/Render/ShaderBuilder.h>
#include <NoFlowers/Render/VertexBufferBuilder.h>
#include <NoFlowers/World/Chunk.h>
#include <NoFlowers/World/World.h>
#include <NoFlowers/Interface/Camera.h>
#include <NoFlowers/Math/MatrixUtils.h>

#define SHADOW_SIZE 8192

static const char* kShadowShaderVertex = R"(
#version 150

in vec3 vPosition;

uniform mat4 mvp;

void main()
{
    gl_Position = vec4(vPosition, 1.0) * mvp;
}
)";

static const char* kShadowShaderFragment = R"(
#version 150

void main()
{
}
)";

static const char* kShaderVertex = R"(
#version 150

in vec3 vPosition;
in vec3 vNormal;
in vec4 vColor;

uniform mat4 mvp;
uniform mat4 shadowMvp;
uniform mat4 model;

out vec4 shadowPosition;
out vec4 fColor;
out float shadowBias;

void main()
{
    vec3 lightDir = normalize(vec3(1.0, 1.0, -0.86));
    float bias = 0.002 * tan(acos(dot(-vNormal, lightDir)));
    shadowBias = clamp(bias, 0, 0.0002);
    float light = mix(0.6, 1.0, max(dot(lightDir, -vNormal), 0.0));
    fColor = vColor * light;
    shadowPosition = ((vec4(vPosition, 1.0) * model * shadowMvp) + 1.0) * 0.5;
    gl_Position = vec4(vPosition, 1.0) * mvp;
}
)";

static const char* kShaderFragment = R"(
#version 150

in vec4 fColor;
in vec4 shadowPosition;
in float shadowBias;

uniform sampler2DShadow shadowTexture;

out vec4 outColor;

void main()
{
    vec2 shadowTexelSize = 1.0 / textureSize(shadowTexture, 0);
    float light = 0;
    for (int x = -2; x <= 2; ++x)
    {
        for (int y = -2; y <= 2; ++y)
        {
            light += texture(shadowTexture, vec3(shadowPosition.xy + vec2(x, y) * shadowTexelSize, shadowPosition.z - shadowBias));
        }
    }
    light /= 25.0;
    outColor = fColor * ((light * 0.5) + 0.5);
}
)";

Renderer::Renderer()
{
    _initShaders();
    _initShadows();
}

Renderer::~Renderer()
{

}

int timeOfDay = 0;

void Renderer::render(const World& world, const Camera& camera)
{
    timeOfDay++;

    Matrix4f view;
    Matrix4f projection;
    Matrix4f viewProj;
    Matrix4f shadowView;
    Matrix4f shadowProjection;
    Matrix4f shadowViewProj;
    Matrix4f shadowMvp;

    view = camera.viewMatrix();
    projection = camera.projectionMatrix();

    shadowView = Matrix4f::identity();
    rotate(shadowView, Vector3f(1, 0, 0), 90);
    rotate(shadowView, Vector3f(1, 0, 0), -60);
    rotate(shadowView, Vector3f(0, 0, 1), 45);
    translate(shadowView, Vector3f(-80, -80, 0));
    shadowProjection = orthogonalProjection(-113, 113, -113, 113, -150.f, 200.f);

    viewProj = projection * view;
    shadowViewProj = shadowProjection * shadowView;
    shadowMvp = shadowViewProj;

    /* Render shadows */
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, _shadowFramebuffer);
    glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
    glClear(GL_DEPTH_BUFFER_BIT);
    _shadowShader.bind();
    _render(world, shadowViewProj, _shadowShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 800, 600);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, _shadowTexture);
    _shader.bind();
    _shader.uniform(ShaderUniform::ShadowMVP, shadowMvp);
    _shader.uniform(ShaderUniform::ShadowTexture, 0);
    _render(world, viewProj, _shader);
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

    ShaderBuilder shadowBuilder;

    shadowBuilder.addSource(ShaderType::Vertex, kShadowShaderVertex);
    shadowBuilder.addSource(ShaderType::Fragment, kShadowShaderFragment);

    shadowBuilder.bindAttribLocation("vPosition", 0);
    _shadowShader = shadowBuilder.link();
}

void Renderer::_initShadows()
{
    glGenTextures(1, &_shadowTexture);
    glBindTexture(GL_TEXTURE_2D, _shadowTexture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32, SHADOW_SIZE, SHADOW_SIZE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    glGenFramebuffers(1, &_shadowFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _shadowFramebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _shadowTexture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void Renderer::_render(const Chunk& chunk, int cx, int cy, const Matrix4f& viewProj, const Shader& shader)
{
    Matrix4f model;
    Matrix4f mvp;

    model = Matrix4f::identity();
    translate(model, Vector3f(cx * CHUNK_X, cy * CHUNK_Y, 0));
    shader.uniform(ShaderUniform::Model, model);

    mvp = viewProj * model;

    if (chunk._dirty)
    {
        _prepareChunk(chunk);
        chunk._dirty = false;
    }

    shader.uniform(ShaderUniform::MVP, mvp);
    chunk._vertexBuffer.bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)chunk._vertexBuffer.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::_render(const World& world, const Matrix4f& viewProj, const Shader& shader)
{
    unsigned cx;
    unsigned cy;

    shader.bind();
    for (unsigned i = 0; i < WORLD_X * WORLD_Y; ++i)
    {
        cx = i % WORLD_X;
        cy = i / WORLD_X;

        _render(*world._chunks[i], cx, cy, viewProj, shader);
    }
}
