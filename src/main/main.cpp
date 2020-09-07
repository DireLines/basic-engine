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
    Matrix3 a_mat = Transform::Translate(200, 50) * Transform::Rotate(3.1415 / 4);
    CircleCollider* a_col = new CircleCollider();
    a_col->radius = 30;
    // Matrix3 b_mat;
    Matrix3 b_mat = Transform::Translate(50, 200) * Transform::Rotate(3.1415 / 4);
    CircleCollider* b_col = new CircleCollider();
    b_col->radius = 50;
    MinkowskiDifferenceSupport s(a_mat, a_col, b_mat, b_col);


    for (int i = 0; i < 360; ++i) {
        Vector2 dir = Transform::Rotate(MathUtils::deg2rad(i)) * Vector2(1, 0);
        Vector2 v = s.transformedSupport(dir, a_mat, a_col);
        Point * plotpoint = new Point();
        plotpoint->getComponent<Transform>()->position = v;
        instantiate(plotpoint);
    }
    for (int i = 0; i < 360; ++i) {
        Vector2 dir = Transform::Rotate(MathUtils::deg2rad(i)) * Vector2(1, 0);
        Vector2 v = s.transformedSupport(dir, b_mat, b_col);
        Point * plotpoint = new Point();
        plotpoint->getComponent<Transform>()->position = v;
        instantiate(plotpoint);
    }
    for (int i = 0; i < 360; ++i) {
        Vector2 dir = Transform::Rotate(MathUtils::deg2rad(i)) * Vector2(1, 0);
        Vector2 v = s.sum(dir);
        Point * plotpoint = new Point();
        plotpoint->getComponent<Transform>()->position = v;
        instantiate(plotpoint);
    }
    // int resolution = 10;
    // int spacing = 950 / resolution;
    // for (int i = 0; i < resolution; ++i) {
    //     for (int j = 0; j < resolution; ++j) {
    //         Square* s = new Square();
    //         instantiate(s);
    //         s->getComponent<Transform>()->position =
    //             Vector2(
    //                 (i - (resolution / 2)) * spacing,
    //                 (j - (resolution / 2)) * spacing
    //             );
    //     }
    // }
}