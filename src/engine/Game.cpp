#include <iostream>
#include "Game.h"
#include <string>
#include <ctime>

using namespace std;

SDL_Renderer* Game::renderer;
Game* Game::instance;
unsigned int Game::frameCounter = 0;

Game::Game(int windowWidth, int windowHeight) {
    Game::instance = this;
    IDGenerator::instance = new IDGenerator();
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    systems.push_back(new ScriptRunner());
    systems.push_back(new Renderer());

    initSDL();
}

Game::~Game() {
    averageFrameLength /= frameCounter;
    cout << "average frame took " << averageFrameLength << " milliseconds" << endl;
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
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Game",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->windowWidth, this->windowHeight, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

    Game::renderer = renderer;
}

void Game::start() {
    int ms_per_frame = (1.0 / (double)this->frames_per_sec) * 1000;
    for (int i = 0; i < 100; ++i) {
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
            update();
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
    for (System* system : systems) {
        system->update();
    }
    frameCounter++;
}

//TODO: call recursively on child objects
void Game::instantiate(GameObject* obj) {
    objects.insert(obj);
    for (System* system : systems) {
        system->maybeAddObject(obj);
    }
}
void Game::destroy(GameObject* obj) {
    for (System* system : systems) {
        system->removeObject(obj);
    }
    objects.erase(obj);
    delete obj;
}