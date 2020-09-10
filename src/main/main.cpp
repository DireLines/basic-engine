#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Game.h"

using namespace std;

int main(int argc, char ** argv) {
    Game* game = new Game(1600, 1000);
    game->start();

    delete game;
    return 0;
}

//game-specific initialization code
void Game::initialize() {
    SDL_SetRenderDrawColor(renderer, 39, 40, 34, 255);

    int num_objects = 200;
    for (int i = 0; i < num_objects; ++i) {
        Circle* s1 = new Circle();
        instantiate(s1);
    }
}