#include "opcodes.h"
#include "chip8.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>

t_status process_opcode(uint16_t *opcode, chip8_t *c)
{
	uint16_t dig = *opcode >> 12;
	printf("%04x : ", *opcode);
	switch(dig)
	{
		case 0x0:
			return process_0(opcode, c);
			break;
		case 0x1:
            return jmp(opcode, c);
            break;
		case 0x2:
            return call_sub(opcode, c);
			break;
		case 0x3:
			return skip_eq_val(opcode, c);
			break;
		case 0x4:
            return skip_neq_val(opcode, c);
			break;
		case 0x5:
            return skip_eq_reg(opcode, c);
			break;
		case 0x6:
            return load_val(opcode, c);
			break;
		case 0x7:
			return add_val(opcode, c);
			break;
		case 0x8:
			return process_8(opcode, c);
			break;
		case 0x9:
            return skip_neq_reg(opcode, c);
			break;
		case 0xa:
            return load_i(opcode, c);
			break;
		case 0xb:
			break;
		case 0xc:
			break;
		case 0xd:
			break;
		case 0xe:
			break;
		case 0xf:
			break;

	}
}

/* --- Nibble 0 --- */
/*
    Process 0's prefixed opcodes
*/
t_status process_0(uint16_t *opcode, chip8_t *c)
{
    uint16_t subop = *opcode & 0xFFF;
    switch(subop)
    {
        case 0xE0:
            return cls_screen(c);
            break;
        case 0xEE:
            return sub_ret(c);
            break;
        default:
            printf("SYS addr : Jump to machine code routine at %03x\n", subop);
            break;
    }
    return SUCCESS;
}

/*
    00E0 - CLS
    Clear the display.
*/
t_status cls_screen(chip8_t *c)
{
    printf("CLS : Clearing screen.\n");
    for(int x = 0; x < WIDTH; x++)
    {
        for(int y = 0; y < HEIGHT; y++)
        {
            c->display[x][y] = 0;
        }
    }
    SDL_SetRenderDrawColor(c->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // Setting color to black
    SDL_RenderClear(c->renderer);
    SDL_RenderPresent(c->renderer);
    return SUCCESS;
}

/*
    00EE - RET
    Return from a subroutine.
    The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
*/
t_status sub_ret(chip8_t *c)
{
    /* Set new program counter */
    c->pc = c->stack[c->sp];
    /* Decrement PC */
    c->sp--;
    printf("RET : PC is now %0x (%d) and stack pointer is %d\n", c->pc, c->pc, c->sp);
    return SUCCESS;
}



/* --- Nibble 1 --- */

/*
    1nnn - JP addr
    Jump to location nnn.
    The interpreter sets the program counter to nnn.
*/
t_status jmp(uint16_t *opcode, chip8_t *c)
{
    uint16_t subop = *opcode & 0xFFF;
    c->pc = subop;
    printf("JMP addr : jumping to %0x (%d)\n", c->pc, c->pc); 
    return SUCCESS;
}

/* --- Nibble 2 --- */

/*
    2nnn - CALL addr
    Call subroutine at nnn.
    The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
*/
t_status call_sub(uint16_t *opcode, chip8_t *c)
{
    // Get address
    uint16_t subop = *opcode & 0xFFF;
    // Increment stack pointer
    c->sp++;
    if(c->sp >= MAX_STACK)
    {
        return STACK_OVERFLOW;
    }
    c->stack[c->sp] = c->pc;
    c->pc = subop;
   printf("CALL %03x instruction : stack pointer is now %d, pointing on %0x. PC is now %0x(%d)\n", subop, c->sp, c->stack[c->sp], c->pc, c->pc);
    return SUCCESS;
}


/* --- Nibble 3 --- */

/*
    3xkk - SE Vx, byte
    Skip next instruction if Vx = kk.
    The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
*/
t_status skip_eq_val(uint16_t *opcode, chip8_t *c)
{
    /* Getting value to check and register index  */
    uint8_t value = *opcode & 0xFF;
    uint8_t reg = *opcode >> 8 & 0xF;
    printf("SE Vx, byte : testing register V%0x value (%0x) against %0x ", reg, c->V[reg], value);
    
    /* Checking match */
    if(c->V[reg] == value)
    {   
        c->pc += 2;
        printf("values are the same, skipping next instruction.\n");
    }
    else
    {
        printf("values are different, no skip.\n");
    }
    return SUCCESS;
}



/* --- Nibble 4 --- */

/*
    4xkk - SNE Vx, byte
    Skip next instruction if Vx != kk.
    The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
*/
t_status skip_neq_val(uint16_t *opcode, chip8_t *c)
{
    /* Getting value to check and register index  */
    uint8_t value = *opcode & 0xFF;
    uint8_t reg = *opcode >> 8 & 0xF;
    printf("SNE Vx, byte : testing register V%0x value (%0x) against %0x ", reg, c->V[reg], value);
    
    /* Checking match */
    if(c->V[reg] != value)
    {   
        c->pc += 2;
        printf("values are different, skipping next instruction.\n");
    }
    else
    {
        printf("values are the same, no skip.\n");
    }
    return SUCCESS;
}

/* --- Nibble 5 --- */

/*
    5xy0 - SE Vx, Vy
    Skip next instruction if Vx = Vy.
    The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
*/
t_status skip_eq_reg(uint16_t *opcode, chip8_t *c)
{
    uint8_t reg_x = (*opcode & 0x0F00) >> 8;
    uint8_t reg_y = (*opcode & 0x00F0) >> 4;
    printf("SE Vx, Vy : testing register V%0x value (%0x) against register V%0x value (%0x) ", reg_x, c->V[reg_x], reg_y, c->V[reg_y]);
    if(c->V[reg_x] == c->V[reg_y])
    {
        c->pc += 2;       
        printf("values are the same, skipping next instruction.\n");
    }
    else
    {  
        printf("values are different, no skip.\n");
    }
    return SUCCESS;
}

/* --- Nibble 6 --- */

/*
    6xkk - LD Vx, byte
    Set Vx = kk.
    The interpreter puts the value kk into register Vx.
*/
t_status load_val(uint16_t *opcode, chip8_t *c)
{
    c->V[(*opcode & 0x0F00) >> 8] = *opcode & 0x00FF;
    printf("LD Vx, byte : Loaded value %0x in V[%0x]\n", c->V[(*opcode & 0x0F00) >> 8], (*opcode & 0x0F00) >> 8);
    return SUCCESS;
}

/* --- Nibble 7 --- */
/*
 	7xkk - ADD Vx, byte
	Set Vx = Vx + kk.
	Adds the value kk to the value of register Vx, then stores the result in Vx. 
*/
t_status add_val(uint16_t *opcode, chip8_t *c)
{
	printf("ADD Vx, byte : Adding %0x to V[%d] ", *opcode & 0xFF,(*opcode & 0xF00) >> 8);
	c->V[(*opcode & 0xF00) >> 8] += *opcode & 0xFF;
	printf("-> register value is now %0x.\n", c->V[(*opcode & 0xF00) >> 8]);
	return SUCCESS;
}

/* --- Nibble 8 --- */
t_status process_8(uint16_t *opcode, chip8_t *c)
{
	switch(*opcode & 0xF)
	{
		case 0x0:
			return load_reg(opcode,c);
			break;
		case 0x1:
			return or_reg(opcode,c);
			break;
		case 0x2:
			return and_reg(opcode,c);
			break;
		case 0x3:
			return xor_reg(opcode,c);
			break;
		case 0x4:
			return add_reg(opcode,c);
			break;
		case 0x5:
			return sub_reg_xy(opcode,c);
			break;
		case 0x6:
			return shr_reg(opcode,c);
			break;
		case 0x7:
			return sub_reg_yx(opcode,c);
			break;
		case 0xe:
			return shl_reg(opcode,c);
			break;
	}
}

/*
 	8xy0 - LD Vx, Vy
	Set Vx = Vy.
	Stores the value of register Vy in register Vx.
*/
t_status load_reg(uint16_t *opcode, chip8_t *c)
{
	printf("LD Vx, Vy : Setting the value of V[%0x] (%0x) on value of V[%0x] (%0x). ", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], (*opcode & 0xF0) >> 4, c->V[(*opcode & 0xF0) >> 4]);
	c->V[(*opcode & 0xF00) >> 8] = c->V[(*opcode & 0xF0) >> 4];
	printf("Value has been set on %0x.\n", c->V[(*opcode & 0xF00) >> 8]);
	return SUCCESS;
}

/*
	8xy1 - OR Vx, Vy
	Set Vx = Vx OR Vy.
	Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0. 
*/
t_status or_reg(uint16_t *opcode, chip8_t *c)
{
	printf("OR Vx, Vy : ORing the value of V[%0x] (%0x) with value of V[%0x] (%0x). ", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], (*opcode & 0xF0) >> 4, c->V[(*opcode & 0xF0) >> 4]);
	c->V[(*opcode & 0xF00) >> 8] |= c->V[(*opcode & 0xF0) >> 4];
	printf("Value has been set on %0x.\n", c->V[(*opcode & 0xF00) >> 8]);
	return SUCCESS;
}

/*
	8xy2 - AND Vx, Vy
	Set Vx = Vx AND Vy.
	Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0. 
*/
t_status and_reg(uint16_t *opcode, chip8_t *c)
{
	printf("AND Vx, Vy : ANDing the value of V[%0x] (%0x) with value of V[%0x] (%0x). ", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], (*opcode & 0xF0) >> 4, c->V[(*opcode & 0xF0) >> 4]);
    c->V[(*opcode & 0xF00) >> 8] &= c->V[(*opcode & 0xF0) >> 4];
    printf("Value has been set on %0x.\n", c->V[(*opcode & 0xF00) >> 8]);
	return SUCCESS;
}

/*
	8xy3 - XOR Vx, Vy
	Set Vx = Vx XOR Vy.
	Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
*/
t_status xor_reg(uint16_t *opcode, chip8_t *c)
{
	printf("XOR Vx, Vy : XORing the value of V[%0x] (%0x) with value of V[%0x] (%0x). ", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], (*opcode & 0xF0) >> 4, c->V[(*opcode & 0xF0) >> 4]);
    c->V[(*opcode & 0xF00) >> 8] ^= c->V[(*opcode & 0xF0) >> 4];
    printf("Value has been set on %0x.\n", c->V[(*opcode & 0xF00) >> 8]);
	return SUCCESS;
}

/*
	8xy4 - ADD Vx, Vy
	Set Vx = Vx + Vy, set VF = carry.
	The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
*/
t_status add_reg(uint16_t *opcode, chip8_t *c)
{
	printf("ADD Vx, Vy : adding the value of V[%0x] (%0x) with value of V[%0x] (%0x). ", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], (*opcode & 0xF0) >> 4, c->V[(*opcode & 0xF0) >> 4]);
	
	/* Adding and put result in X register and carry register (VF)*/
	uint16_t res = c->V[(*opcode & 0xF00) >> 8] + c->V[(*opcode & 0xF0) >> 4];
	c->V[(*opcode & 0xF00) >> 8] = res & 0xFF;
	c->V[0xF] = (res & 0xF00) >> 8;

	printf("Values has been set on V[%0x]: %0x, VF: %d.\n", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], c->V[0xF]);
	return SUCCESS;
}

/*
    8xy5 - SUB Vx, Vy
    Set Vx = Vx - Vy, set VF = NOT borrow.
    If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
*/
t_status sub_reg_xy(uint16_t *opcode, chip8_t *c)
{
    printf("SUB Vx, Vy : substracting the value of V[%0x] (%0x) from value of V[%0x] (%0x). ", (*opcode & 0xF0) >> 4, c->V[(*opcode & 0xF0) >> 4], (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8]);

    // Set VF 
    c->V[0xF] = c->V[(*opcode & 0xF00) >> 8] > c->V[(*opcode & 0xF0) >> 4] ? 1 : 0;
    // Substract
    c->V[(*opcode & 0xF00) >> 8] -= c->V[(*opcode & 0xF0) >> 4];

    if(c->V[0xF])
    {
        printf("There was no underflow, V[%0x] is now %0x and V[F] is %d.\n", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], c->V[0xF]);      
    }
    else
    {
        printf("UNDERFLOW, V[%0x] is now %0x and V[F] is %d.\n", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], c->V[0xF]);      
    }
	return SUCCESS;
}

/*
    DISCLAIMER : I'm ignoring Vy as intended in original chip-8 (See SCHIP for use of Vy)
    8xy6 - SHR Vx {, Vy}
    Set Vx = Vx SHR 1.
    If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
*/
t_status shr_reg(uint16_t *opcode, chip8_t *c)
{
    printf("SHR Vx {, Vy} : Dividing V[%0x] (%0x) by 2. ", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8]);
    
    // Setting VF
    c->V[0xF] = c->V[(*opcode & 0xF00) >> 8] & 0x1;
    // Dividing by 2
    c->V[(*opcode & 0xF00) >> 8] = c->V[(*opcode & 0xF00) >> 8] >> 1;
    
    printf("V[%0x] is now %0x. VF has been set on %d.\n", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], c->V[0xF]);
	return SUCCESS;
}


/*
    8xy7 - SUBN Vx, Vy
    Set Vx = Vy - Vx, set VF = NOT borrow.
    If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
*/
t_status sub_reg_yx(uint16_t *opcode, chip8_t *c)
{
    printf("SUBN Vx, Vy : substracting the value of V[%0x] (%0x) from value of V[%0x] (%0x). ", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], (*opcode & 0xF0) >> 4, c->V[(*opcode & 0xF0) >> 4]);

    // Set VF 
    c->V[0xF] = c->V[(*opcode & 0xF0) >> 4] > c->V[(*opcode & 0xF00) >> 8] ? 1 : 0;
    // Substract
    c->V[(*opcode & 0xF00) >> 8] = c->V[(*opcode & 0xF0) >> 4] - c->V[(*opcode & 0xF00) >> 8];

    if(c->V[0xF])
    {
        printf("There was no underflow, V[%0x] is now %0x and V[F] is %d.\n", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], c->V[0xF]);      
    }
    else
    {
        printf("UNDERFLOW, V[%0x] is now %0x and V[F] is %d.\n", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], c->V[0xF]);      
    }
	return SUCCESS;
}

/*
    DISCLAIMER : I'm ignoring Vy as intended in original chip-8 (See SCHIP for use of Vy)
    8xyE - SHL Vx {, Vy}
    Set Vx = Vx SHL 1.
    If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
*/
t_status shl_reg(uint16_t *opcode, chip8_t *c)
{
    printf("SHL Vx {, Vy} : Multiplying V[%0x] (%0x) by 2. ", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8]);
    
    // Setting VF
    c->V[0xF] = (c->V[(*opcode & 0xF00) >> 8] & 128) ? 1 : 0;
    // Dividing by 2
    c->V[(*opcode & 0xF00) >> 8] = c->V[(*opcode & 0xF00) >> 8] << 1;
    
    printf("V[%0x] is now %0x. VF has been set on %d.\n", (*opcode & 0xF00) >> 8, c->V[(*opcode & 0xF00) >> 8], c->V[0xF]);
	return SUCCESS;
}

/* Nibble 9 */

/*
    9xy0 - SNE Vx, Vy
    Skip next instruction if Vx != Vy.
    The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
*/
t_status skip_neq_reg(uint16_t *opcode, chip8_t *c)
{
    uint8_t reg_x, reg_y;
    reg_x = (*opcode & 0xF00) >> 8;
    reg_y = (*opcode & 0xF0) >> 4;
    printf("SNE Vx, Vy : testing register V%0x value (%0x) against register V%0x value (%0x) ", reg_x, c->V[reg_x], reg_y, c->V[reg_y]);
    if(c->V[(*opcode & 0xF00) >> 8] != c->V[(*opcode & 0xF0) >> 4])
    {
        printf("Values are not equal, skipping next instruction.\n");
        c->pc += 2;
    }
    else
    {
        printf("Values are equal, no skip.\n");
    }
    return SUCCESS;
}

/* Nibble A */
/*
    Annn - LD I, addr
    Set I = nnn.
    The value of register I is set to nnn.
*/
t_status load_i(uint16_t *opcode, chip8_t *c)
{
    printf("LD I, addr : Setting reg I to ");
    c->I = *opcode & 0XFFF;
    printf(" %0x.\n", c->I);
    return SUCCESS;
}

/* Nibble B */
t_status jmp_plus(uint16_t *opcode, chip8_t *c);    // JP V0, addr

/* Nibble C */
t_status rnd_and(uint16_t *opcode, chip8_t *c);     // RND Vx, byte

/* Nibble D */
/*
    Dxyn - DRW Vx, Vy, nibble
    Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
*/
t_status draw(uint16_t *opcode, chip8_t *c)
{
    //SDL_Ren
}

/* Nibble E */
t_status process_e(uint16_t *opcode, chip8_t *c);
t_status skip_prsd(uint16_t *opcode, chip8_t *c);   // SKP Vx
t_status skip_nprsd(uint16_t *opcode, chip8_t *c);  // SKNP Vx

/* Nibble F */
t_status process_f(uint16_t *opcode, chip8_t *c);
t_status load_dt(uint16_t *opcode, chip8_t *c);     // LD Vx, DT
t_status get_key(uint16_t *opcode, chip8_t *c);     // LD Vx, K
t_status set_dt(uint16_t *opcode, chip8_t *c);      // LD DT, Vx
t_status set_st(uint16_t *opcode, chip8_t *c);      // LD ST, Vx
t_status add_i_reg(uint16_t *opcode, chip8_t *c);   // ADD I, Vx
t_status load_sprite(uint16_t *opcode, chip8_t *c); // LD F, Vx
t_status get_decimal(uint16_t *opcode, chip8_t *c); // LD B, Vx
t_status store_reg(uint16_t *opcode, chip8_t *c);   // LD [I], Vx
t_status read_reg(uint16_t *opcode, chip8_t *c);    // LD, Vx, [I]






