#include "opcodes.h"

t_status process_opcode(uint16_t *opcode, chip8_t *c)
{
	uint16_t dig = *opcode >> 12;
	printf("%04x : ", *opcode);
	switch(dig)
	{
		case 0x0:
			process_0(opcode, c);
			break;
		case 0x1:
            jmp(opcode, c);
            break;
		case 0x2:
            call_sub(opcode, c);
			break;
		case 0x3:
			skip_eq_val(opcode, c);
			break;
		case 0x4:
            skip_neq_val(opcode, c);
			break;
		case 0x5:
            skip_eq_reg(opcode, c);
			break;
		case 0x6:
            load_val(opcode, c);
			break;
		case 0x7:
			add_val(opcode, c);
			break;
		case 0x8:
			break;
		case 0x9:
			break;
		case 0xa:
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
    printf("CLS : Please implement cls_screen().\n");
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

/* Nibble 8 */
t_status process_8(uint16_t *opcode, chip8_t *c);
t_status load_reg(uint16_t *opcode, chip8_t *c);    // LD Vx, Vy
t_status or_reg(uint16_t *opcode, chip8_t *c);      // OR Vx, Vy
t_status and_reg(uint16_t *opcode, chip8_t *c);     // AND Vx, Vy
t_status xor_reg(uint16_t *opcode, chip8_t *c);     // XOR Vx, Vy
t_status add_reg(uint16_t *opcode, chip8_t *c);     // ADD Vx, Vy
t_status sub_reg(uint16_t *opcode, chip8_t *c);     // SUB Vx, Vy
t_status shr_reg(uint16_t *opcode, chip8_t *c);     // SHR Vx {, Vy}
t_status sub_reg_nb(uint16_t *opcode, chip8_t *c);  // SUBN Vx, Vy
t_status shl_reg(uint16_t *opcode, chip8_t *c);     // SHL Vx {, Vy}

/* Nibble 9 */
t_status skip_neq_reg(uint16_t *opcode, chip8_t *c);// SNE Vx, Vy

/* Nibble A */
t_status load_i(uint16_t *opcode, chip8_t *c);      // LD I, addr

/* Nibble B */
t_status jmp_plus(uint16_t *opcode, chip8_t *c);    // JP V0, addr

/* Nibble C */
t_status rnd_and(uint16_t *opcode, chip8_t *c);     // RND Vx, byte

/* Nibble D */
t_status draw(uint16_t *opcode, chip8_t *c);        // DRW Vx, Vy, nibble

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






