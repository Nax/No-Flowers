#ifndef GAME_GAME_STATE_H
#define GAME_GAME_STATE_H 1

#include <SDL2/SDL.h>
#include <NoFlowers/Util/NonCopyable.h>

struct Game;

class GameState : private NonCopyable
{
public:
    GameState(Game& game): game(game) { }
    virtual ~GameState() {}

    virtual void init() = 0;
    virtual void deinit() = 0;
    virtual void update() = 0;
    virtual void handleEvent(const SDL_Event& e) = 0;
    virtual void render(float dt) = 0;

protected:
    Game& game;
};

#endif
