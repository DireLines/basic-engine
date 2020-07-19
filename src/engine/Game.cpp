#include <iostream>
#include "Game.h"
#include <string>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

SDL_Renderer* Game::renderer;
Game* Game::instance;
unsigned int Game::frameCounter = 0;

Game::Game(int windowWidth, int windowHeight) {
    Game::instance = this;
    IDGenerator::instance = new IDGenerator();
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    scriptRunner = new ScriptRunner();

    initSDL();
    TTF_Init();
}

Game::~Game() {
    averageFrameLength /= frameCounter;
    double averageFPS = 1000.0 / averageFrameLength;
    cout << "target FPS: " << frames_per_sec << endl;
    cout << "actual FPS: " << averageFPS << endl;
    quitSDL();
}

void Game::quitSDL() {
    cout << "Quitting sdl" << endl;
    SDL_DestroyRenderer(Game::renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

void Game::initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Game",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->windowWidth, this->windowHeight, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    Game::renderer = renderer;
}

void Game::start() {
    int ms_per_frame = (1.0 / (double)this->frames_per_sec) * 1000;
    for (int i = 0; i < 10000; ++i) {
        instantiate(new Square());
    }
    std::clock_t start = std::clock();

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        std::clock_t end = std::clock();
        double duration = (( end - start ) / (double) CLOCKS_PER_SEC) * 1000;
        if (duration > ms_per_frame) {
            averageFrameLength += duration;
            start = end;
            //call systems
            this->update();
            this->draw();
        }

        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            pressedKeys.insert(event.key.keysym.scancode);
            break;
        case SDL_KEYUP:
            pressedKeys.erase(event.key.keysym.scancode);
            break;
        }

    }
}

void Game::update() {
    scriptRunner->update();
    frameCounter++;
}
void Game::draw() {

}

void Game::instantiate(GameObject* obj) {
    objects.insert(obj);
    //TODO: attach obj to all the other lists that care about it
    if (obj->hasComponent<Script>()) {
        scriptRunner->addObject(obj);
    }
}

void Game::destroy(GameObject* obj) {
    objects.erase(obj);
    //TODO: remove obj from all the other lists that care about it
    if (obj->hasComponent<Script>()) {
        scriptRunner->removeObject(obj);
    }
    delete obj;
}