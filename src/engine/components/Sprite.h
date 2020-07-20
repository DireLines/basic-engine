#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
using namespace std;
class Sprite: public Component {
public:
    //TODO: figure out how to represent the actual sprite
    string filename;
    double z; //rendering order
    double alpha = 1.0;
private:
    SDL_Surface* image = NULL;
    SDL_Texture* texture = NULL;
    friend class Renderer;
};

#endif