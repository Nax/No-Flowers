#include <NoFlowers/Game.h>

int main(int argc, char** argv)
{
    Game game;

    gameInit(&game);
    gameLoop(&game);
    gameQuit(&game);

    return 0;
}
