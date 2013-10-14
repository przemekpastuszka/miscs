#ifndef PP_GAME_APP
#define PP_GAME_APP

#include <SDL/SDL.h>
#include "resources.h"

class App{
    protected:
    int width, height;
    int running;
    
    SDL_Surface* display;
    ResourceManager resources;
    
    
    int init();
    int handle_event(SDL_Event* event);
    int update();
    int render();
    int cleanup();
    
    public:
    int execute();
    App(int w, int h);
};

#endif
