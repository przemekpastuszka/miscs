#include "app.h"
#include <SDL/SDL_image.h>

int App :: cleanup(){
    //SDL_FreeSurface(display);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
