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
    Matrix3 a_mat = Transform::Rotate(3.1415 / 4);
    BoxCollider* a_col = new BoxCollider();
    a_col->width = 100;
    a_col->height = 1;
    // Matrix3 b_mat;
    Matrix3 b_mat = Transform::Translate(5, 5) * Transform::Rotate(3.1415 / 4);
    CircleCollider* b_col = new CircleCollider();
    MinkowskiDifferenceSupport s(a_mat, a_col, b_mat, b_col);
    Vector2 dir(1, 1);
    s(dir).print();
    SDL_SetRenderDrawColor(renderer, 39, 40, 34, 255);
    int resolution = 10;
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
        }
    }
}