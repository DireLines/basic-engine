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
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            Square* s = new Square();
            instantiate(s);
            s->getComponent<Transform>()->position = Vector2((i - 50) * 9, (j - 50) * 9);
        }
    }
}