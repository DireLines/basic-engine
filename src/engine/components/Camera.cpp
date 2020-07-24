#include "Camera.h"
#include "Game.h"
#include "Spin.h"

Camera::Camera() {
    Transform* t = this->addComponent<Transform>();
}