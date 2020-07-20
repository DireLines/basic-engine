#include "Spin.h"
#include <iostream>
void Spin::update() {
    gameObject->getComponent<Transform>()->rotation += speed;
}
