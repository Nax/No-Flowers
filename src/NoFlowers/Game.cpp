#include <chrono>
#include <NoFlowers/Game.h>
#include <NoFlowers/Render/OpenGL.h>
#include <NoFlowers/Render/ShaderBuilder.h>

static const char* kShaderVertex = R"(
#version 150

in vec3 vPosition;
in vec4 vColor;

out vec4 fColor;

void main()
{
    fColor = vColor;
    gl_Position = vec4(vPosition, 1.0);
}
)";

static const char* kShaderFragment = R"(
#version 150

in vec4 fColor;

out outColor;

void main()
{
    outColor = fColor;
}
)";

static void gameInitShader(Game* game)
{
    ShaderBuilder builder;

    builder.addSource(ShaderType::Vertex, kShaderVertex);
    builder.addSource(ShaderType::Fragment, kShaderFragment);

    builder.bindAttribLocation("vPosition", 0);
    builder.bindAttribLocation("vColor", 1);

    game->shader = builder.link();
}

void gameInit(Game* g)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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
    SDL_GL_SetSwapInterval(1);
    g->camera.setPerspective(90.f, 800.f / 600.f, 0.1f, 1000.f);
    gameInitShader(g);
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
}

void gameRender(Game* game, float dt)
{
    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    SDL_GL_SwapWindow(game->window);
}
