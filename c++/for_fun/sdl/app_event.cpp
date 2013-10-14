#include "app.h"

int App :: handle_event(SDL_Event* event){
    if(event -> type == SDL_QUIT){
        running = 0;
    }
    return 0;
}
