#ifndef OPCODES_H
#define OPCODES_H

#include "chip8.h"
t_status process_opcode(uint16_t *opcode, chip8_t *c);

/* Nibble 0 */
t_status process_0(uint16_t *opcode, chip8_t *c);   
t_status cls_screen(chip8_t *c);                    // CLS
t_status sub_ret(chip8_t *c);                       // RET

/* Nibble 1 */
t_status jmp(uint16_t *opcode, chip8_t *c);         // JP addr

/* Nibble 2 */
t_status call_sub(uint16_t *opcode, chip8_t *c);    // CALL addr

/* Nibble 3 */
t_status skip_eq_val(uint16_t *opcode, chip8_t *c); // SE Vx, byte

/* Nibble 4 */
t_status skip_neq_val(uint16_t *opcode, chip8_t *c);// SNE Vx, byte

/* Nibble 5 */
t_status skip_eq_reg(uint16_t *opcode, chip8_t *c); // SE Vx, Vy

/* Nibble 6 */
t_status load_val(uint16_t *opcode, chip8_t *c);    // LD Vx, byte

/* Nibble 7 */
t_status add_val(uint16_t *opcode, chip8_t *c);     // ADD Vx, byte

/* Nibble 8 */
t_status process_8(uint16_t *opcode, chip8_t *c);
t_status load_reg(uint16_t *opcode, chip8_t *c);    // LD Vx, Vy
t_status or_reg(uint16_t *opcode, chip8_t *c);      // OR Vx, Vy
t_status and_reg(uint16_t *opcode, chip8_t *c);     // AND Vx, Vy
t_status xor_reg(uint16_t *opcode, chip8_t *c);     // XOR Vx, Vy
t_status add_reg(uint16_t *opcode, chip8_t *c);     // ADD Vx, Vy
t_status sub_reg_xy(uint16_t *opcode, chip8_t *c);     // SUB Vx, Vy
t_status shr_reg(uint16_t *opcode, chip8_t *c);     // SHR Vx {, Vy}
t_status sub_reg_yx(uint16_t *opcode, chip8_t *c);  // SUBN Vx, Vy
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


#endif
