#include "app.h"

int App :: render(){
    SDL_Surface* surf = resources.load_image("big.png");
    SDL_Rect r;
    r.x = r.y = 0;
    SDL_BlitSurface(surf, NULL, display, &r);
    SDL_Flip(display);
    
    return 0;
}
