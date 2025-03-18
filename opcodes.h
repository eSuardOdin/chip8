#ifndef OPCODES_H
#define OPCODES_H

#include "chip8.h"
t_status process_opcode(uint16_t *opcode);
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

#endif
