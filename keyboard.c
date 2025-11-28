#include "keyboard.h"
/*
123C    1234
456D    AZER
789E    QSDF
A0BF    WXCV

*/

void update_keyboard(SDL_KeyboardEvent* key, chip8_t* c)
{
    bool status = key->type == SDL_KEYDOWN ? true : false; 
    switch(key->keysym.sym)
    {
        case SDLK_1: c->keys[0x1] = status; return;
        case SDLK_2: c->keys[0x2] = status; return;
        case SDLK_3: c->keys[0x3] = status; return;
        case SDLK_4: c->keys[0xC] = status; return;
        case SDLK_a: c->keys[0x4] = status; return;
        case SDLK_z: c->keys[0x5] = status; return;
        case SDLK_e: c->keys[0x6] = status; return;
        case SDLK_r: c->keys[0xD] = status; return;
        case SDLK_q: c->keys[0x7] = status; return;
        case SDLK_s: c->keys[0x8] = status; return;
        case SDLK_d: c->keys[0x9] = status; return;
        case SDLK_f: c->keys[0xE] = status; return;
        case SDLK_w: c->keys[0xA] = status; return;
        case SDLK_x: c->keys[0x0] = status; return;
        case SDLK_c: c->keys[0xB] = status; return;
        case SDLK_v: c->keys[0xF] = status; return;
        default: return;
    }
}