#include "chip8.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
/* Inits the struct in a base status */
t_status init_chip8(chip8_t *c)
{
	// TODO : Handle NULL error
	memset(c, 0, sizeof(chip8_t));
	// Add here special cases (stack pointer ?)
	return SUCCESS;
}

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
//	*opcode = (c->ram[c->pc+1] << 8) | c->ram[c->pc];
	*opcode = (c->ram[c->pc] << 8) | c->ram[c->pc+1];
}

