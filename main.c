/*
 * Just trying to fetch instructions and display them
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */

#include "chip8.h"
#include "opcodes.h"
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(int argc, char* argv[])
{
	t_status status = 0;
    chip8_t *chip8 = malloc(sizeof(chip8_t));		// VM
	uint16_t opcode;    							// Fetched opcode
    FILE *fp;           							// ROM (well, not read only actually...)
    
    /* Open ROM */
    fp = fopen(argv[1], "rb");
    if(fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    /* Init chip8 struct */
	init_chip8(chip8);
    /* Load ROM into RAM */
    if((status = load_rom(chip8, fp)))
    {
        // Error handling todo
        exit(EXIT_FAILURE);
    }


    
    run_chip8(chip8);
    
    exit(EXIT_SUCCESS);
}

