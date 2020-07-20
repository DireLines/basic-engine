#include "Camera.h"

Camera::Camera() {
    this->enabled = false;
    this->addComponent<Transform>();
}