#include "Reproduce.h"
#include "Game.h"
#include "GameObjects.h"

void Reproduce::update() {
    if (rand() % 600 == 0) {
        Cell* s = new Cell();
        Game::instance->instantiate(s);
        s->getComponent<Transform>()->position = gameObject->getComponent<Transform>()->position;
    }
    if (rand() % 1000 == 0) {
        Game::instance->destroy(gameObject);
    }
}