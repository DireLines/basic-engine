#include "Spin.h"
#include <iostream>
void Spin::update() {
    // cout << gameObject->getComponent<Transform>()->rotation << endl;
    gameObject->getComponent<Transform>()->rotation += speed;
}
