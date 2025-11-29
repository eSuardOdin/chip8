#include "chip8.h"
#include "opcodes.h"
#include "keyboard.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_audio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define BEEP_FREQUENCY 440

static volatile bool running = true;


// To keep track of beep phase
static float phase = 0.0f;
static pthread_t decrement_timers_thread;

// Create a global mutex
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

/* Inits the struct in a base status */
t_status init_chip8(chip8_t *c)
{

	pthread_create(&decrement_timers_thread, NULL, decrement_timers, (void*) c);
	// TODO : Find a better place to init rnd, maybe.
	// Init RND
	srand(time(NULL));
	// TODO : Handle NULL error
	memset(c, 0, sizeof(chip8_t));
	// Init SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO))
        return SDL_INIT_ERROR;
    // Create Window
    c->window = SDL_CreateWindow("My CHIP-8 Emulator", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WIDTH * PIX_SIZE, HEIGHT * PIX_SIZE, 0);
    if(c->window == NULL)
	{
		SDL_Quit();
        return SDL_WINDOW_CREATION_ERROR;
	}
	// Create renderer
	c->renderer = SDL_CreateRenderer(c->window, -1, 0);
	if(c->renderer == NULL)
	{
		SDL_DestroyWindow(c->window);
		SDL_Quit();
		return SDL_RENDERER_CREATION_ERROR;
	}

	
	// Stack pointer and Program counter -> See doc for 0x200
    c->sp = -1;
	c->pc = 0x200; 

	// Init keys
	for(int i = 0; i < 16; i++) c->keys[i] = false;
	// Init sdl audio
	SDL_AudioSpec spec;
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 512;
    spec.callback = beep_callback;
    spec.userdata = &c->s_timer;
    
    SDL_OpenAudio(&spec, NULL);
    SDL_PauseAudio(0);


	// Show window
	SDL_ShowWindow(c->window);
	
	return SUCCESS;
}

/* Pop the stack */
/*t_status pop_stack(chip8_t *c)
{


}*/


/* Free chip8, just a placeholder for now */
t_status free_chip8(chip8_t *c)
{
	pthread_join(decrement_timers_thread, NULL);
	SDL_CloseAudio();
    SDL_DestroyRenderer(c->renderer);
    SDL_DestroyWindow(c->window);
    pthread_mutex_destroy(&mut);
	SDL_Quit();
	return SUCCESS;
}



/* Loads ROM into VM RAM */
t_status load_rom(chip8_t *c, FILE *fp)
{
	int i = 0;
	while(fread(&(c->ram[0x200 + i]), 1, 1, fp) == 1)
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
	//*opcode = (c->ram[c->pc+1] << 8) | c->ram[c->pc]; // Inverted
	*opcode = (c->ram[c->pc] << 8) | c->ram[c->pc+1]; // Not inverted
}


t_status run_chip8(chip8_t *c)
{
	running = true;
	SDL_Event event;
	uint16_t opcode;
	SDL_RenderClear(c->renderer);
	SDL_RenderPresent(c->renderer);
	
	while(running)
	{
		pthread_mutex_lock(&mut);
		fetch_instruction(c, &opcode);
		t_status status = process_opcode(&opcode, c);
		pthread_mutex_unlock(&mut);
		// Poll event
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running = false;;
					break;
				case SDL_KEYUP:
				case SDL_KEYDOWN:
					update_keyboard(&event.key, c);
					break;
				default: break;
			}
		}
		if(status != PC_MODIFIED) {
			c->pc += 2;
		}
		usleep(2000);	// Put frequency as a parameter
	}
	
	free_chip8(c);
	return SUCCESS;
}


void beep_callback(void* userdata, uint8_t* stream, int len)
{
	uint8_t* sound_timer = (uint8_t*)userdata;
	pthread_mutex_lock(&mut);
	// Check if sound register is > 0
	bool is_sound = *sound_timer;
	pthread_mutex_unlock(&mut);

	if(is_sound)
	{
		generate_beep(stream, len);
	}
	else
	{
		memset(stream, 0, len);
	}
}


void generate_beep(uint8_t* stream, int len) {
    int16_t* buffer = (int16_t*)stream;
    int samples = len / 2;  // len est en bytes, on veut des samples (16-bit = 2 bytes)
    
    for(int i = 0; i < samples; i++) {
        // Onde carrée simple : alterne entre -amplitude et +amplitude
        float sample = (phase < 0.5f) ? 3000 : -3000;  // Amplitude modérée
        buffer[i] = (int16_t)sample;
        
        // Incrémenter la phase
        phase += BEEP_FREQUENCY / 44100.0f;
        if(phase >= 1.0f) phase -= 1.0f;
    }
}

void *decrement_timers(void *chip) {
    struct timespec ts = {.tv_sec = 0, .tv_nsec = 16666666};
    chip8_t* c = (chip8_t *)chip;
    while(running) {
        pthread_mutex_lock(&mut);
		if(c->s_timer)
		{
			// printf("SOUND TIMER IS %d.\n", c->s_timer);
			c->s_timer--;
		}
		if(c->d_timer)
		{
			c->d_timer--;
		}
        pthread_mutex_unlock(&mut);

        clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
    }
}