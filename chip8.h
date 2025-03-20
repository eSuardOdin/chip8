/*
 *
 * This file defines structs 
 * and constants used by my 
 * chip8 emulator
 *
 */

#ifndef CHIP8_H
#define CHIP8_H

#include <SDL2/SDL_render.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// --- Constants ---
#define MAX_RAM 	4096
#define REG_N		16
#define MAX_STACK 	16
#define WIDTH		64
#define HEIGHT		32
#define PIX_SIZE	10

// --- Structs and types ---

/* Base struct, represents the VM */
typedef struct
{
	uint8_t 	ram[MAX_RAM];		// RAM
	uint8_t 	V[REG_N];		    // Registers
	uint16_t	I;					// Special register	*usually storing adresses so only 12 bits really used*
	uint16_t 	stack[MAX_STACK];  	// Stack
	uint16_t 	pc;					// Program counter
	uint8_t 	sp;					// Stack pointer
	uint8_t		s_timer;			// Sound timer register
	uint8_t		d_timer;			// Delay timer register
	uint16_t	keys;				// Keys 			*I'll use a bit per key (16 of them)*
	//uint16_t 	opcode;				// Current loaded instruction
	uint8_t		display[WIDTH][HEIGHT];
	SDL_Renderer *renderer;
	SDL_Window *window;
} chip8_t;

/* Error status enum */
typedef enum {
	SUCCESS,
	RAM_OVERFLOW,
	STACK_OVERFLOW,
	SDL_INIT_ERROR,
	SDL_WINDOW_CREATION_ERROR,
	SDL_RENDERER_CREATION_ERROR
	} t_status;


// --- Functions ---

/* Inits the struct in a base status */
t_status init_chip8(chip8_t *c);
/* Free memory allocatied for chip8 */
t_status free_chip8(chip8_t *c);
/* Loads ROM into VM RAM */
t_status load_rom(chip8_t *c, FILE *fp);
/* Load instruction corresponding to current pc */
t_status fetch_instruction(chip8_t *c, uint16_t *opcode);
/* Main run loop */
t_status run_chip8(chip8_t *c);

#endif
