#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include "Game.h"

using namespace std;

int main(int argc, char ** argv) {
    Game* game = new Game(1200, 800);
    game->start();

    delete game;
    return 0;
}

//game-specific initialization code
void Game::initialize() {
    SDL_SetRenderDrawColor(renderer, 39, 40, 34, 255);

    for (int i = 0; i < 200; ++i) {
        Square* s = new Square();
        instantiate(s);
    }
    instantiate(new Player());
    for (int i = 0; i < 2; ++i) {
        instantiate(new CircleWall());
    }
}