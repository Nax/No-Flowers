#ifndef GAME_H
#define GAME_H

#include <cstdint>
#include <SDL2/SDL.h>
#include <NoFlowers/Interface/Camera.h>
#include <NoFlowers/Render/Shader.h>
#include <NoFlowers/Render/VertexBuffer.h>
#include <NoFlowers/Game/GameStateMain.h>

struct Game
{
    bool            running;
    GameStateMain*  stateMain;
    GameState*      stateActive;
    SDL_Window*     window;
    SDL_GLContext   gl;
    Camera          camera;
    Shader          shader;
    VertexBuffer    vb;
};

void gameInit(Game* game);
void gameQuit(Game* game);
void gameLoop(Game* game);
void gameUpdate(Game* game);
void gameRender(Game* game, float dt);

#endif
