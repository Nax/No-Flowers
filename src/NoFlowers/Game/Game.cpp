#include <chrono>
#include <NoFlowers/Game/Game.h>
#include <NoFlowers/Render/OpenGL.h>

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
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

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
    g->camera.setPerspective(90.f, 800.f / 600.f, 0.01f, 1000.f);
    g->renderer = new Renderer;
    g->stateMain = new GameStateMain(*g);
    g->stateActive = g->stateMain;
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
    game->stateActive->init();
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
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            game->running = false;
        game->stateActive->handleEvent(e);
    }

    game->stateActive->update();
}

void gameRender(Game* game, float dt)
{
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    game->stateMain->render(dt);
    glBindVertexArray(0);
    SDL_GL_SwapWindow(game->window);
}
