#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
using namespace std;
class Sprite: public Component {
public:
    string file = "default.png";
    double z; //rendering order
    double alpha = 1.0;
    // glm::vec3 color(255,255,255);
    SDL_Color color = {255, 255, 255};
private:
    SDL_Surface* image = NULL;
    SDL_Texture* texture = NULL;
    friend class Renderer;
};

#endif