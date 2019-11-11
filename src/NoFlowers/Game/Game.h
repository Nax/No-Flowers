#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <SDL2/SDL.h>
#include <NoFlowers/Interface/Camera.h>
#include <NoFlowers/Render/Shader.h>
#include <NoFlowers/Render/Renderer.h>
#include <NoFlowers/Render/VertexBuffer.h>
#include <NoFlowers/Game/GameStateMain.h>

struct Game
{
    bool            running;
    GameStateMain*  stateMain;
    GameState*      stateActive;
    SDL_Window*     window;
    SDL_GLContext   gl;
    Renderer*       renderer;
    Camera          camera;
};

void gameInit(Game* game);
void gameQuit(Game* game);
void gameLoop(Game* game);
void gameUpdate(Game* game);
void gameRender(Game* game, float dt);

#endif
