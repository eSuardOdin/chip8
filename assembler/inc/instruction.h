#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "common.h"

/**
 * @brief Enum describing CHIP 8's registers
 * 
 */
typedef enum {
    REG_V0,
    REG_V1,
    REG_V2,
    REG_V3,
    REG_V4,
    REG_V5,
    REG_V6,
    REG_V7,
    REG_V8,
    REG_V9,
    REG_VA,
    REG_VB,
    REG_VC,
    REG_VD,
    REG_VE,
    REG_VF,
    REG_I,
    REG_I_INDIRECTION,
    REG_F,              // I alias
    REG_B,              // I alias
    REG_DELAY_TIMER,
    REG_SOUND_TIMER
} Register;

/**
 * @brief Possible instruction's arguments type
 * 
 */
typedef enum {
    ARG_V_REGISTER,
    ARG_I_REGISTER,
    ARG_I_INDIRECT,
    ARG_TIME_REGISTER,  // Can have 'dt' (delay timer) or 'st' (sound timer) values
    ARG_KEY,
    ARG_ADDRESS,
    ARG_BYTE,
    ARG_NIBBLE,
    ARG_ERROR
} ArgType;

/**
 * @brief Defines an argument type and value
 * 
 */
typedef struct {
    ArgType type;
    union {
        Register reg;
        uint16_t address;
        uint8_t byte;
        uint8_t nibble;
    } as;
} Argument;

/**
 * @brief Defines an opcode type
 * 
 */
typedef enum {
    OP_CLS,
    OP_RET,
    OP_SYS,
    OP_JP,
    OP_CALL,
    OP_SE,
    OP_SNE,
    OP_LD,
    OP_ADD,
    OP_OR,
    OP_AND,
    OP_XOR,
    OP_SUB,
    OP_SHR,
    OP_SUBN,
    OP_SHL,
    OP_RND,
    OP_DRW,
    OP_SKP,
    OP_SKNP,
    //OP_SCD,
    //OP_SCR,
    //OP_SCL,
    //OP_EXIT,
    //OP_LOW,
    //OP_HIGH,
    OP_ERROR
} Opcode;

/**
 * @brief Defines a full instruction
 * 
 */
typedef struct {
    Opcode opcode;
    Argument* args;
    int args_count;
    int args_capacity;
} Instruction;

/**
 * @brief Defines a valid suite of arg types
 * to associate to an opcode
 * 
 */
typedef struct {
    ArgType *args;
    int arg_count;
} ArgSuite;


typedef struct {
    Opcode opcode;
    ArgSuite** suites;
    uint16_t* binaries;
    int suites_count;
} InstructionValidator;



void add_argument(Instruction* inst, Argument* arg);
void init_instruction(Instruction* inst);
void init_valid_instructions();
#endif