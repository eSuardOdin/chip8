#include "chip8.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
/* Inits the struct in a base status */
t_status init_chip8(chip8_t *c)
{
	// Init SDL
    if(SDL_Init(SDL_INIT_VIDEO))
        return SDL_INIT_ERROR;
    // Create Window
    SDL_Window* window = SDL_CreateWindow("My CHIP-8 Emulator", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WIDTH * PIX_SIZE, HEIGHT * PIX_SIZE, 0);
    if(window == NULL)
        return SDL_WINDOW_CREATION_ERROR;
	// Create renderer
	c->renderer = SDL_CreateRenderer(window, -1, 0);
	if(c->renderer == NULL)
		return SDL_RENDERER_CREATION_ERROR;

	// TODO : Handle NULL error
	memset(c, 0, sizeof(chip8_t));
    c->sp = -1;
	return SUCCESS;
}

/* Pop the stack */
/*t_status pop_stack(chip8_t *c)
{


}*/


/* Free chip8, just a placeholder for now */
t_status free_chip8(chip8_t *c)
{
	free(c);
	return SUCCESS;
}

/* Loads ROM into VM RAM */
t_status load_rom(chip8_t *c, FILE *fp)
{
	int i = 0;
	while(fread(&(c->ram[i]), 1, 1, fp) == 1)
	{
		if(i >= MAX_RAM)
		{
            fprintf(stderr, "Ram overflow !\n");
            return RAM_OVERFLOW;
        }
		i++;
	}
	return SUCCESS;
}
/* 
	Load instruction corresponding to current pc 
	(put it in little endian?)
*/
t_status fetch_instruction(chip8_t *c, uint16_t *opcode)
{
	// *opcode = (c->ram[c->pc+1] << 8) | c->ram[c->pc]; // Inverted
	*opcode = (c->ram[c->pc] << 8) | c->ram[c->pc+1]; // Not inverted
}

