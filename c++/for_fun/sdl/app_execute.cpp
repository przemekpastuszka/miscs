#include <cassert>
#include "app.h"
#include "common.h"

int App :: execute(){
    assert(init() == 0);
    
    SDL_Event event;
    
    while(running){
        while(SDL_PollEvent(&event)){
            assert(handle_event(&event) == 0);
        }
        
        assert(update() == 0);
        assert(render() == 0);
    }
    
    assert(cleanup() == 0);
    
    return 0;
}
