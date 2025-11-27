#include "display.h"
#include "chip8.h"
#include <SDL2/SDL.h>

t_status init_display()
{
    // Init SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        return SDL_INIT_ERROR;
    }
    
}