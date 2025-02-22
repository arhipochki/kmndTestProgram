#include "Game.h"

const char* TITLE = "DEMO";

const int WIDTH  = 262;
const int HEIGHT = 460; 

int main(void)
{
    Game game(WIDTH, HEIGHT, TITLE);
    game.Run();

    return 0;
}