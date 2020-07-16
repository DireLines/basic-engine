#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <DisplayObject.h>
#include <iostream>
#include "Game.h"

using namespace std;

int main(int argc, char ** argv) {
    Game* game = new Game(1200, 1000);
    game->start();

    delete game;
    return 0;
}