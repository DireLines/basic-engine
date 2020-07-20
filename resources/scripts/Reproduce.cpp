#include "Reproduce.h"
#include "Game.h"
#include "GameObjects.h"

void Reproduce::update() {
    // if (rand() % 1000 == 0) {
    //     Cell* s = new Cell();
    //     s->getComponent<Transform>()->position = gameObject->getComponent<Transform>()->position;
    //     Game::instance->instantiate(s);
    // }
    if (rand() % 1000 == 0) {
        Game::instance->destroy(gameObject);
    }
}