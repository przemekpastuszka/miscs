#include <SDL/SDL_image.h>
#include <cassert>
#include "app.h"
#include "common.h"

App :: App(int w, int h) : width(w), height(h){
    display = 0;
    running = 1;
};

int App :: init(){
    assert(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == 0);
    display = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    assert(display != NULL);
    assert(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG));
    
    return 0;
}
