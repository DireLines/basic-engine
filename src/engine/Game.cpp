#include <iostream>
#include "Game.h"
#include <string>
#include "WallTime.h"
#include "ElapsedTimeLogger.h"

using namespace std;

SDL_Renderer* Game::renderer;
Game* Game::instance;
unsigned int Game::frameCounter = 0;

Game::Game(int windowWidth, int windowHeight) {
    Game::instance = this;
    IDGenerator::instance = new IDGenerator();
    GameTimer::instance = new GameTimer();
    Input::instance = new Input();
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    //apply forces
    //update position/velocity
    systems.push_back(new PhysicsSystem());

    //detect collisions
    //resolve collisions
    systems.push_back(new CollisionSystem());
    //call collision events

    //run scripts
    systems.push_back(new ScriptRunner());

    //draw results
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
    for (System* system : systems) {
        // cout << system->getName() << " start" << endl;
        system->start();
    }
    initialize();
    double ms_per_frame = (1.0 / (double)this->frames_per_sec) * 1000;
    auto start = get_wall_time();
    GameTimer::time = 0;
    bool quit = false;

    while (!quit) {
        auto end = get_wall_time();
        double duration = (end - start) * 1000;
        if (duration > ms_per_frame) {
            averageFrameLength += duration;
            start = end;
            GameTimer::deltaTime = duration / 1000;
            GameTimer::time += GameTimer::deltaTime;
            Input::update();
            quit = update();
            Input::postUpdate();
        }
    }
}

bool Game::update() {
    SDL_Event event;
    ElapsedTimeLogger logElapsedTime  = ElapsedTimeLogger("update");
    for (System* system : systems) {
        system->update();
        logElapsedTime(system->getName());
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
            return true;
            break;
        default:
            Input::poll(event);
            break;
        }
    }
    std::for_each(objectsToDelete.begin(), objectsToDelete.end(), [](GameObject * obj) {
        delete obj;
    });
    objectsToDelete.clear();
    frameCounter++;
    return false;
}

void Game::instantiate(GameObject* obj) {
    // cout << "instantiate " << obj->name << endl;
    objects.insert(obj);
    for (GameObject* child : obj->getChildren()) {
        instantiate(child);
    }
    for (System* system : systems) {
        // cout << system->getName() << " add" << endl;
        system->maybeAddObject(obj);
    }
}
void Game::destroy(GameObject* obj) {
    for (System* system : systems) {
        // cout << system->getName() << " remove" << endl;
        system->removeObject(obj);
    }
    for (GameObject* child : obj->getChildren()) {
        destroy(child);
    }
    objects.erase(obj);
    objectsToDelete.insert(obj);
}