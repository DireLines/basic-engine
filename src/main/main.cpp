#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Game.h"

using namespace std;

int main(int argc, char ** argv) {
    Game* game = new Game(1600, 1200);
    game->start();

    delete game;
    return 0;
}

void Game::initialize() {
    for (int i = 0; i < 2000; ++i) {
        instantiate(new Square());
    }
}