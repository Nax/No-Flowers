#ifndef GAME_GAME_STATE_MAIN_H
#define GAME_GAME_STATE_MAIN_H 1

#include <NoFlowers/Game/GameState.h>
#include <NoFlowers/World/Chunk.h>

class GameStateMain : public GameState
{
public:
    GameStateMain(Game& game) : GameState(game) {}

    void init() override;
    void deinit() override;
    void update() override;
    void handleEvent(const SDL_Event& e) override;
    void render(float dt) override;

private:
    Chunk* _chunk;
};

#endif
