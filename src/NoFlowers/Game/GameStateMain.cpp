#include <NoFlowers/Game/Game.h>
#include <NoFlowers/Game/GameStateMain.h>

void GameStateMain::init()
{
    SDL_SetRelativeMouseMode(SDL_TRUE);
    _world = new World;
    _worldGenerator = new WorldGenerator(*_world, 0);
    _worldGenerator->generate();
}

void GameStateMain::deinit()
{

}

void GameStateMain::update()
{
    Vector2f motion;
    Vector3f motionCamera;

    const uint8_t* kb = SDL_GetKeyboardState(NULL);

    if (kb[SDL_SCANCODE_A])
        motion.x -= 0.1f;
    if (kb[SDL_SCANCODE_D])
        motion.x += 0.1f;
    if (kb[SDL_SCANCODE_W])
        motion.y += 0.1f;
    if (kb[SDL_SCANCODE_S])
        motion.y -= 0.1f;

    motionCamera = game.camera.forward() * motion.y + game.camera.side() * motion.x;
    game.camera.move(motionCamera);
}

void GameStateMain::handleEvent(const SDL_Event& e)
{
    const float sensitivityX = 15.f;
    const float sensitivityY = sensitivityX * 800.f / 600.f;
    Vector2f camRotate;

    if (e.type == SDL_MOUSEMOTION)
    {
        camRotate.x += e.motion.xrel / sensitivityX;
        camRotate.y += e.motion.yrel / sensitivityY;
    }
    game.camera.rotate(camRotate);
}

void GameStateMain::render(float dt)
{
    game.renderer->render(*_world, game.camera);
}
