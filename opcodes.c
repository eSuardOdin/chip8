#include "opcodes.h"

t_status process_opcode(uint16_t *opcode)
{
	uint16_t dig = *opcode >> 12;
	printf("%04x (%d) : ", *opcode, dig);
	switch(dig)
	{
		case 0x0:
			printf("0\n");
			break;
		case 0x1:
			printf("1\n");
			break;
		case 0x2:
			printf("2\n");
			break;
		case 0x3:
			printf("3\n");
			break;
		case 0x4:
			printf("4\n");
			break;
		case 0x5:
			printf("5\n");
			break;
		case 0x6:
			printf("6\n");
			break;
		case 0x7:
			printf("7\n");
			break;
		case 0x8:
			printf("8\n");
			break;
		case 0x9:
			printf("9\n");
			break;
		case 0xa:
			printf(" 10\n");
			break;
		case 0xb:
			printf(" 11\n");
			break;
		case 0xc:
			printf(" 12\n");
			break;
		case 0xd:
			printf(" 13\n");
			break;
		case 0xe:
			printf(" 14\n");
			break;
		case 0xf:
			printf(" 15\n");
			break;

	}
}
/*
t_status process_0(uint16_t *opcode);
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
