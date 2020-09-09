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

    int resolution = 15;
    int spacing = 950 / resolution;
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            Square* s = new Square();
            instantiate(s);
            s->getComponent<Transform>()->position =
                Vector2(
                    (i - (resolution / 2)) * spacing,
                    (j - (resolution / 2)) * spacing
                );
            Circle* c = new Circle();
            instantiate(c);
            c->getComponent<Transform>()->position =
                Vector2(
                    (i - (resolution / 2)) * spacing,
                    (j - (resolution / 2)) * spacing
                );
        }
    }
}