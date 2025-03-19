#include "opcodes.h"

t_status process_opcode(uint16_t *opcode)
{
	uint16_t dig = *opcode >> 12;
	printf("%04x : ", *opcode);
	switch(dig)
	{
		case 0x0:
			process_0(opcode);
			break;
		case 0x1:
			//process_1(opcode);  // JP addr
            jmp(opcode);
            break;
		case 0x2:
			//process_2(opcode);  // CALL addr
            call_sub(opcode);
			break;
		case 0x3:
			skip_eq_val(opcode);
			break;
		case 0x4:
			break;
		case 0x5:
			break;
		case 0x6:
			break;
		case 0x7:
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

t_status process_0(uint16_t *opcode)
{
    uint16_t subop = *opcode & 0xFFF;
    switch(subop)
    {
        case 0xE0:
            printf("CLS instruction : Please implement cls_screen().\n");
            break;
        case 0xEE:
            printf("RET instruction : Please implement sub_ret().\n");
            break;
        default:
            printf("SYS addr : Jump to machine code routine at %03x\n", subop);
            break;
    }
    return SUCCESS;
}

t_status jmp(uint16_t *opcode)
{
    uint16_t subop = *opcode & 0xFFF;
    printf("JP %03x instruction : Please implement jmp(uint16_t addr).\n", subop);
    return SUCCESS;
}

t_status call_sub(uint16_t *opcode)
{
    uint16_t subop = *opcode & 0xFFF;
    printf("CALL %03x instruction : Please implement call_sub(uint16_t addr).\n", subop);
    return SUCCESS;
}

t_status skip_eq_val(uint16_t *opcode)
{
    uint8_t value = *opcode & 0xFF;
    uint8_t reg = *opcode >> 8 & 0xF;
    printf("SE V%0x, %0x instruction : Please implement skip_eq(uint8_t reg, uint8_t value).\n", reg, value);
    return SUCCESS;
}










/*
t_status process_1(uint16_t *opcode);
t_status process_2(uint16_t *opcode);
t_status process_3(uint16_t *opcode);
t_status process_4(uint16_t *opcode);
t_status process_5(uint16_t *opcode);
t_status process_6(uint16_t *opcode);
t_status process_7(uint16_t *opcode);
t_status process_8(uint16_t *opcode);
t_status process_9(uint16_t *opcode);
t_status process_a(uint16_t *opcode);
t_status process_b(uint16_t *opcode);
t_status process_c(uint16_t *opcode);
t_status process_d(uint16_t *opcode);
t_status process_e(uint16_t *opcode);
t_status process_f(uint16_t *opcode);
*/
