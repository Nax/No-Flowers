#include <cstdio>
#include <chrono>
#include <NoFlowers/Game.h>
#include <NoFlowers/Render/OpenGL.h>
#include <NoFlowers/Render/ShaderBuilder.h>
#include <NoFlowers/Render/VertexBufferBuilder.h>
#include <NoFlowers/World/Block.h>
#include <NoFlowers/World/Material.h>


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

static FILE* debugFile;

static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    fprintf(debugFile, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

static void gameDisableDebug()
{
    fclose(debugFile);
}

static void gameEnableDebug()
{
    debugFile = fopen("debug.txt", "w");
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&MessageCallback, 0);
    atexit(&gameDisableDebug);
}

static void gameInitShader(Game* game)
{
    ShaderBuilder builder;

    builder.addSource(ShaderType::Vertex, kShaderVertex);
    builder.addSource(ShaderType::Fragment, kShaderFragment);

    builder.bindAttribLocation("vPosition", 0);
    builder.bindAttribLocation("vNormal", 1);
    builder.bindAttribLocation("vColor", 2);

    game->shader = builder.link();
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

static void makeCube(VertexBufferBuilder& builder, Vector3f pos)
{
    Vector4f color = Vector4f(((pos / 2.f) + 5.f) / 10.f, 1.f);

    makeQuad(builder, Vector3f(0.f, 0.f, -1.f), color, pos, Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f));
    makeQuad(builder, Vector3f(0.f, -1.f, 0.f), color, pos, Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 0.f, 1.f));
    makeQuad(builder, Vector3f(-1.f, 0.f, 0.f), color, pos, Vector3f(0.f, 1.f, 0.f), Vector3f(0.f, 0.f, 1.f));
    makeQuad(builder, Vector3f( 1.f, 0.f, 0.f), color, pos + Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f), Vector3f(0.f, 0.f, 1.f));
    makeQuad(builder, Vector3f(0.f,  1.f, 0.f), color, pos + Vector3f(0.f, 1.f, 0.f), Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 0.f, 1.f));
    makeQuad(builder, Vector3f(0.f, 0.f,  1.f), color, pos + Vector3f(0.f, 0.f, 1.f), Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f));
}

static void gameInitVertexBuffer(Game* game)
{
    VertexBufferBuilder builder(game->vb);

    builder.attr(0, 3);
    builder.attr(1, 3);
    builder.attr(2, 4);

    for (int x = 0; x < 10; ++x)
    {
        for (int y = 0; y < 10; ++y)
        {
            for (int z = 0; z < 10; ++z)
            {
                Vector3f pos(x, y, z);
                pos -= 5;
                pos *= 2;
                makeCube(builder, pos);
            }
        }
    }

    builder.submit();
}

void gameInit(Game* g)
{
    Material::init();
    Block::init();

    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

    g->window = SDL_CreateWindow(
        "No Flowers",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    g->gl = SDL_GL_CreateContext(g->window);
    glewInit();
    gameEnableDebug();
    SDL_GL_SetSwapInterval(1);
    g->camera.setPerspective(90.f, 800.f / 600.f, 0.1f, 1000.f);
    gameInitShader(g);
    gameInitVertexBuffer(g);
}

void gameQuit(Game* g)
{
    SDL_GL_DeleteContext(g->gl);
    SDL_DestroyWindow(g->window);
    SDL_Quit();
}


void gameLoop(Game* game)
{
    static const uint64_t updateFreq = 100;
    static const uint64_t updatePeriod = (1000000000 / 100);

    using Clock = std::chrono::high_resolution_clock;
    using Duration = std::chrono::duration<uint64_t, std::nano>;
    using TimePoint = std::chrono::time_point<Clock, Duration>;

    TimePoint now;
    TimePoint last;
    uint64_t acc;

    game->running = true;
    acc = 0;
    last = Clock::now();
    while (game->running)
    {
        gameRender(game, 0.f);
        now = Clock::now();
        acc += (now - last).count();
        last = now;
        while (acc >= updatePeriod)
        {
            acc -= updatePeriod;
            gameUpdate(game);
        }
    }
}

void gameUpdate(Game* game)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            game->running = false;
    }

    game->camera.move(Vector3f(0.f, 0.01f, 0.f));
}

void gameRender(Game* game, float dt)
{
    Matrix4f model;
    Matrix4f view;
    Matrix4f projection;
    Matrix4f mvp;

    model = Matrix4f::identity();
    view = game->camera.viewMatrix();
    projection = game->camera.projectionMatrix();

    mvp = projection * view;

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    game->shader.bind();
    game->shader.uniform(ShaderUniform::MVP, mvp);
    game->vb.bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)game->vb.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    SDL_GL_SwapWindow(game->window);
}
