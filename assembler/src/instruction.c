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