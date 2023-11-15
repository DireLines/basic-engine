#include <iostream>
#include "Game.h"
#include "Linmath.h"
#include <string>
#include "WallTime.h"
#include "ElapsedTimeLogger.h"

using namespace std;

static const struct {
    float x, y;
    float r, g, b;
} vertices[3] = {
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};
static const char* vertex_shader_text =
    "#version 110\n"
    "uniform mat4 MVP;\n"
    "attribute vec3 vCol;\n"
    "attribute vec2 vPos;\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char* fragment_shader_text =
    "#version 110\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

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
    glm::vec3 v(2.f, 2.f, 2.f);
    float l = glm::length(v);
    cout << "l is " << l << endl;

    initSDL();
    initGLFW();
}

Game::~Game() {
    averageFrameLength /= frameCounter;
    cout << "average frame took " << averageFrameLength << " milliseconds" << endl;
    double averageFPS = 1000.0 / averageFrameLength;
    cout << "target FPS: " << frames_per_sec << endl;
    cout << "actual FPS: " << averageFPS << endl;
    quitGLFW();
    quitSDL();
}

void Game::quitSDL() {
    cout << "Quitting sdl" << endl;
    SDL_DestroyRenderer(Game::renderer);
    SDL_DestroyWindow(sdl_window);

    IMG_Quit();
    SDL_Quit();
}

void Game::initSDL() {
    TTF_Init();
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    sdl_window = SDL_CreateWindow("Game",
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->windowWidth, this->windowHeight, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(sdl_window, -1, 0);

    Game::renderer = renderer;
}

void Game::initGLFW() {
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(this->windowWidth, this->windowHeight, "Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));
}
void Game::quitGLFW() {
    glfwDestroyWindow(window);
    glfwTerminate();
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
        // TODO: use techniques from https://medium.com/@tglaiel/how-to-make-your-game-run-at-60fps-24c61210fe75
        // while(total_updates < 10000) {
        //     double current_frame_time = system_time;
        //     double delta_frame_time = current_frame_time - prev_frame_time;
        //     accumulator += delta_frame_time;
        //     prev_frame_time = current_frame_time;

        //     while(accumulator >= 1.0/61.0) {
        //         simulate_update();
        //         accumulator -= 1.0/60.0;
        //         if(accumulator < (1.0/59.0 - 1.0/60.0)) accumulator = 0;
        //     }

        //     simulate_render();
        //     simulate_display();
        //     simulate_busy();
        // }
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
        glfwPollEvents();
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