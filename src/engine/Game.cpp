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
    GameTimer::instance = new GameTimer();
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    //eventual goal:

    //run scripts
    systems.push_back(new ScriptRunner());

    //apply forces
    //update position/velocity
    systems.push_back(new PhysicsSystem());

    //detect collisions
    //resolve collisions
    systems.push_back(new CollisionSystem());

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

#ifdef _WIN32
#include <Windows.h>
double get_wall_time() {
    LARGE_INTEGER time, freq;
    if (!QueryPerformanceFrequency(&freq)) {
        //  Handle error
        return 0;
    }
    if (!QueryPerformanceCounter(&time)) {
        //  Handle error
        return 0;
    }
    return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time() {
    FILETIME a, b, c, d;
    if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
        //  Returns total user time.
        //  Can be tweaked to include kernel times as well.
        return
            (double)(d.dwLowDateTime |
                     ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    } else {
        //  Handle error
        return 0;
    }
}

//  Posix/Linux
#else
#include <time.h>
#include <sys/time.h>
double get_wall_time() {
    struct timeval time;
    if (gettimeofday(&time, NULL)) {
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time() {
    return (double)clock() / CLOCKS_PER_SEC;
}
#endif

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
    SDL_Event event;

    while (!quit) {
        auto end = get_wall_time();
        // double duration = ((end - start).count() / (double) CLOCKS_PER_SEC) * 1000;
        double duration = (end - start) * 1000;
        if (duration > ms_per_frame) {
            averageFrameLength += duration;
            start = end;
            GameTimer::deltaTime = duration / 1000;
            GameTimer::time += GameTimer::deltaTime;
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
        // cout << system->getName() << " update" << endl;
        system->update();
    }
    std::for_each(objectsToDelete.begin(), objectsToDelete.end(), [](GameObject * obj) { delete obj; });
    objectsToDelete.clear();
    frameCounter++;
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