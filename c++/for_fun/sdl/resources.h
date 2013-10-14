#ifndef PP_GAME_RESOURCES
#define PP_GAME_RESOURCES

#include <SDL/SDL.h>
#include <map>
#include <string>

class ResourceManager{
    private:
    std :: map<std :: string, SDL_Surface*> images;
    
    public:
    SDL_Surface* load_image(std :: string file);
    
    ~ResourceManager();
};

#endif
