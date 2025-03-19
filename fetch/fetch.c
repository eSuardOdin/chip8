/*
 * Just trying to fetch instructions and display them
 * http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_RAM 4096

int decode(uint16_t *opcode);
int fetch_instruction(int pc, uint16_t *opcode, uint8_t *ramp);
int load_rom(FILE *fp, uint8_t *ram);
typedef enum {SUCCESS, RAM_OVERFLOW} t_status;
int main(int argc, char* argv[])
{
    int status;                 // Error status
    uint8_t ram[MAX_RAM] = {0}; // RAM
    uint8_t V[16];              // Registers
    uint8_t pc = 0;             // Program counter
    uint16_t opcode;            // Fetched opcode
    FILE *fp;                   // ROM (well, not read only actually...)
    
    /* Open ROM */
    fp = fopen(argv[1], "rb");
    if(fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    
    /* Load ROM into RAM */
    if((status = load_rom(fp, ram)))
    {
        // Error handling todo
        exit(EXIT_FAILURE);
    }
    
    /* Fetching test */
    for(pc; pc <= 8; pc += 2)
    {
        fetch_instruction(pc, &opcode, ram);
        printf("%04x\n", opcode);
    }

    exit(EXIT_SUCCESS);
}


int load_rom(FILE *fp, uint8_t *ram)
{
    int ram_i = 0;
    while(fread(&ram[ram_i], 1, 1, fp))
    {
        if(ram_i >= MAX_RAM)
        {
            fprintf(stderr, "Ram overflow !\n");
            return (int)RAM_OVERFLOW;
        }
        ram_i++;
        //opcode = (buffer[1] << 8 | buffer[0]);
        //printf("%02X%02X -> %d\n", buffer[1], buffer[0], opcode);
    }

    return (int)SUCCESS;
}

int fetch_instruction(int pc, uint16_t *opcode, uint8_t *ramp)
{
    *opcode = (ramp[pc+1] << 8) | ramp[pc];
}
