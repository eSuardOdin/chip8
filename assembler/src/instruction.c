#include "../inc/instruction.h"
#include "../inc/memory.h"

void add_argument(Instruction* inst, Argument* arg) {
    if(inst->args_capacity < inst->args_count + 1) {
        GROW_CAPACITY(inst->args_capacity);
        inst->args = GROW_ARRAY(Argument, inst->args, inst->args_capacity);
    }
    inst->args[inst->args_count++] = *arg;
}

void init_instruction(Instruction* inst) {
    inst->args = NULL;
    inst->args_capacity = 0;
    inst->args_count = 0;
    inst->opcode = OP_ERROR;
}

static InstructionValidator valid_instructions[INST_NB];

void init_valid_instructions() {
    // Create arg suites
    
    // 0
    ArgSuite empty;
    empty.arg_count = 0;
    empty.args = NULL;


    // 1
    ArgSuite addr;
    addr.arg_count = 1;
    addr.args = malloc(sizeof(ArgType) * addr.arg_count);
    addr.args[0] = ARG_ADDRESS;

    ArgSuite vreg;
    vreg.arg_count = 1;
    vreg.args = malloc(sizeof(ArgType) * vreg.arg_count);
    vreg.args[0] = ARG_V_REGISTER;


    // 2
    ArgSuite vreg_vreg;
    vreg_vreg.arg_count = 2;
    vreg_vreg.args = malloc(sizeof(ArgType) * vreg_vreg.arg_count);
    vreg_vreg.args[0] = ARG_V_REGISTER;
    vreg_vreg.args[1] = ARG_V_REGISTER;

    ArgSuite vreg_byte;
    vreg_byte.arg_count = 2;
    vreg_byte.args = malloc(sizeof(ArgType) * vreg_byte.arg_count);
    vreg_byte.args[0] = ARG_V_REGISTER;
    vreg_byte.args[1] = ARG_BYTE;

    ArgSuite vreg_addr;
    vreg_addr.arg_count = 2;
    vreg_addr.args = malloc(sizeof(ArgType) * vreg_addr.arg_count);
    vreg_addr.args[0] = ARG_V_REGISTER; // WARNING ! MUST BE V0
    vreg_addr.args[1] = ARG_ADDRESS;

    ArgSuite ireg_addr;
    ireg_addr.arg_count = 2;
    ireg_addr.args = malloc(sizeof(ArgType) * ireg_addr.arg_count);
    ireg_addr.args[0] = ARG_I_REGISTER;
    ireg_addr.args[1] = ARG_ADDRESS;

    ArgSuite vreg_timereg;
    vreg_timereg.arg_count = 2;
    vreg_timereg.args = malloc(sizeof(ArgType) * vreg_timereg.arg_count);
    vreg_timereg.args[0] = ARG_V_REGISTER;
    vreg_timereg.args[1] = ARG_TIME_REGISTER;

    ArgSuite vreg_key;
    vreg_key.arg_count = 2;
    vreg_key.args = malloc(sizeof(ArgType) * vreg_key.arg_count);
    vreg_key.args[0] = ARG_V_REGISTER;
    vreg_key.args[1] = ARG_KEY;

    ArgSuite ireg_vreg;
    ireg_vreg.arg_count = 2;
    ireg_vreg.args = malloc(sizeof(ArgType) * ireg_vreg.arg_count);
    ireg_vreg.args[0] = ARG_I_REGISTER;
    ireg_vreg.args[1] = ARG_V_REGISTER;

    ArgSuite iregind_vreg;
    iregind_vreg.arg_count = 2;
    iregind_vreg.args = malloc(sizeof(ArgType) * iregind_vreg.arg_count);
    iregind_vreg.args[0] = ARG_I_INDIRECT;
    iregind_vreg.args[1] = ARG_V_REGISTER;

    ArgSuite vreg_iregind;
    vreg_iregind.arg_count = 2;
    vreg_iregind.args = malloc(sizeof(ArgType) * vreg_iregind.arg_count);
    vreg_iregind.args[0] = ARG_V_REGISTER;
    vreg_iregind.args[1] = ARG_I_INDIRECT;


    // 3
    ArgSuite vreg_vreg_nibble;
    vreg_vreg_nibble.arg_count = 3;
    vreg_vreg_nibble.args = malloc(sizeof(ArgType) * vreg_vreg_nibble.arg_count);
    vreg_vreg_nibble.args[0] = ARG_V_REGISTER;
    vreg_vreg_nibble.args[1] = ARG_V_REGISTER;
    vreg_vreg_nibble.args[1] = ARG_NIBBLE;

}

// Instruction type :
/*
0:
    - NULL
1:
    - ARG_ADDR
    - ARG_V_REG
2:
    - ARG_V_REG + ARG_V_REG
    - ARG_V_REG + ARG_ADDR  // V reg must be v0, to handle.
    - ARG_V_REG + ARG_BYTE
    - ARG_V


*/