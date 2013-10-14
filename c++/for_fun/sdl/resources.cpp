#include <SDL/SDL_image.h>
#include <cassert>
#include "resources.h"

SDL_Surface* ResourceManager :: load_image(std :: string file){
    std :: map<std :: string, SDL_Surface*> :: iterator element = images.find(file);
    if(element != images.end()){
        return (*element).second;
    }
    SDL_Surface* result = IMG_Load(file.c_str());
    assert(result != NULL);
    images[file] = result;
    return result;
}

ResourceManager :: ~ResourceManager(){
    for(std :: map<std :: string, SDL_Surface*> :: iterator it = images.begin();
        it != images.end(); it++){
            SDL_FreeSurface((*it).second);
        }
}
