#include "../inc/instruction.h"
#include "../inc/memory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


// *** Debug ***
void print_arg_type(ArgType type) {
    switch (type) {
        case ARG_V_REGISTER:    printf("[ARG_V_REGISTER]"); break;
        case ARG_I_REGISTER:    printf("[ARG_I_REGISTER]"); break;
        case ARG_ADDRESS:       printf("[ARG_ADDRESS]"); break;
        case ARG_I_INDIRECT:    printf("[ARG_I_INDIRECT]"); break;
        case ARG_KEY:           printf("[ARG_KEY]"); break;  
        case ARG_BYTE:          printf("[ARG_BYTE]"); break;
        case ARG_NIBBLE:        printf("[ARG_NIBBLE]"); break;
        case ARG_DELAY_TIMER: printf("[ARG_DELAY_TIMER]"); break;
        case ARG_SOUND_TIMER: printf("[ARG_SOUND_TIMER]"); break;
        case ARG_ERROR:         printf("[ARG_ERROR]"); break;
    }
}

void print_opcode(Opcode op) {
    switch (op) {
        case OP_CLS: printf("[OP_CLS]"); break;
        case OP_RET: printf("[OP_RET]"); break;
        case OP_SYS: printf("[OP_SYS]"); break;
        case OP_JP: printf("[OP_JP]"); break;
        case OP_CALL: printf("[OP_CALL]"); break;
        case OP_SE: printf("[OP_SE]"); break;
        case OP_SNE: printf("[OP_SNE]"); break;
        case OP_LD: printf("[OP_LD]"); break;
        case OP_ADD: printf("[OP_ADD]"); break;
        case OP_OR: printf("[OP_OR]"); break;
        case OP_AND: printf("[OP_AND]"); break;
        case OP_XOR: printf("[OP_XOR]"); break;
        case OP_SUB: printf("[OP_SUB]"); break;
        case OP_SHR: printf("[OP_SHR]"); break;
        case OP_SUBN: printf("[OP_SUBN]"); break;
        case OP_SHL: printf("[OP_SHL]"); break;
        case OP_RND: printf("[OP_RND]"); break;
        case OP_DRW: printf("[OP_DRW]"); break;
        case OP_SKP: printf("[OP_SKP]"); break;
        case OP_SKNP: printf("[OP_SKNP]"); break;
        case OP_ERROR: printf("[OP_ERROR]"); break;
    }
}

void print_instruction(Instruction* inst) {
    print_opcode(inst->opcode);
    for(int i = 0; i < inst->args_count; i++) {
        putchar(' ');
        print_arg_type(inst->args[i].type);
    }
    putchar('\n');
}
// ****************************




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

static InstructionValidator *valid_instructions;



static InstructionValidator init_validator(int suites_count, Opcode op) {
    InstructionValidator val;
    val.opcode = op;
    val.suites_count = suites_count;
    val.suites = malloc(sizeof(ArgSuite*) * suites_count);
    return val;
}

void init_encoded_instruction(EncodedInstructions* inst) {
    inst->instructions = NULL;
    inst->count = 0;
    inst->capacity = 0;
}

void add_encoded_instruction(EncodedInstructions* inst, uint16_t bin) {
    if(inst->capacity < inst->count + 1) {
        GROW_CAPACITY(inst->capacity);
        inst->instructions = GROW_ARRAY(uint16_t, inst->instructions, inst->capacity);
    }
    inst->instructions[inst->count++] = bin;
}

static void add_binary(int index, ArgSuite* argsuite, InstructionValidator* validator) {
    validator->suites[index] = argsuite; 
}

/**
 * @brief Initialize all valid arsuites to be tested against
 * 
 */
void init_valid_instructions() {
    printf("[DEBUG] - init_valid_instructions(): Enter function.\n");
    valid_instructions = malloc(sizeof(InstructionValidator) * INST_NB);
    
    
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

    ArgSuite vreg_delaytimer;
    vreg_delaytimer.arg_count = 2;
    vreg_delaytimer.args = malloc(sizeof(ArgType) * vreg_delaytimer.arg_count);
    vreg_delaytimer.args[0] = ARG_V_REGISTER;
    vreg_delaytimer.args[1] = ARG_DELAY_TIMER;

    ArgSuite delaytimer_vreg;
    delaytimer_vreg.arg_count = 2;
    delaytimer_vreg.args = malloc(sizeof(ArgType) * delaytimer_vreg.arg_count);
    delaytimer_vreg.args[0] = ARG_DELAY_TIMER;
    delaytimer_vreg.args[1] = ARG_V_REGISTER;

    ArgSuite soundtimer_vreg;
    soundtimer_vreg.arg_count = 2;
    soundtimer_vreg.args = malloc(sizeof(ArgType) * soundtimer_vreg.arg_count);
    soundtimer_vreg.args[0] = ARG_SOUND_TIMER;
    soundtimer_vreg.args[1] = ARG_V_REGISTER;

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
    vreg_vreg_nibble.args[2] = ARG_NIBBLE;


    printf("[DEBUG] - init_valid_instructions(): Opcodes implementation.\n");


    // Opcodes implementation
    int op_num = 0;
    // - cls
    InstructionValidator cls = init_validator(1, OP_CLS);
    add_binary(0, &empty,&cls);
    valid_instructions[op_num++] = cls;
    // - ret
    InstructionValidator ret = init_validator(1,OP_RET);
    add_binary(0, &empty, &ret);
    valid_instructions[op_num++] = ret;
    // - sys
    InstructionValidator sys = init_validator(1, OP_SYS);
    add_binary(0,&addr,&sys);
    valid_instructions[op_num++] = sys;
    // - jp
    InstructionValidator jp = init_validator(2, OP_JP);
    add_binary(0, &addr, &jp);
    add_binary(1, &vreg_addr, &jp);
    valid_instructions[op_num++] = jp;
    // - call
    InstructionValidator call = init_validator(1, OP_CALL);
    add_binary(0, &addr,&call);
    valid_instructions[op_num++] = call;
    // - se
    InstructionValidator se = init_validator(2, OP_SE);
    add_binary(0, &vreg_byte,&se);
    add_binary(1, &vreg_vreg,&se);
    valid_instructions[op_num++] = se;
    // - sne
    InstructionValidator sne = init_validator(2, OP_SNE);
    add_binary(0, &vreg_byte,&sne);
    add_binary(1, &vreg_vreg,&sne);
    valid_instructions[op_num++] = sne;
    // - ld
    InstructionValidator ld = init_validator(10, OP_LD);
    add_binary(0, &vreg_byte, &ld);
    add_binary(1, &vreg_vreg, &ld);
    add_binary(2, &ireg_addr, &ld);
    add_binary(3, &vreg_delaytimer, &ld);
    add_binary(4, &vreg_key, &ld);
    add_binary(5, &delaytimer_vreg, &ld);
    add_binary(6, &soundtimer_vreg, &ld);
    add_binary(7, &ireg_vreg, &ld);
    add_binary(8, &iregind_vreg, &ld);
    add_binary(9, &vreg_iregind, &ld);
    valid_instructions[op_num++] = ld;
    // - add
    InstructionValidator add = init_validator(3, OP_ADD);
    add_binary(0, &vreg_byte, &add);
    add_binary(1, &vreg_vreg, &add);
    add_binary(2, &ireg_vreg, &add);
    valid_instructions[op_num++] = add;
    // - or
    InstructionValidator or = init_validator(1, OP_OR);
    add_binary(0, &vreg_vreg, &or);
    valid_instructions[op_num++] = or;
    // - and
    InstructionValidator and = init_validator(1, OP_AND);
    add_binary(0, &vreg_vreg,&and);
    valid_instructions[op_num++] = and;
    // - xor
    InstructionValidator xor = init_validator(1, OP_XOR);
    add_binary(0, &vreg_vreg, &xor);
    valid_instructions[op_num++] = xor;
    // - sub
    InstructionValidator sub = init_validator(1, OP_SUB);
    add_binary(0, &vreg_vreg, &sub);
    valid_instructions[op_num++] = sub;
    // - shr
    InstructionValidator shr = init_validator(2, OP_SHR);
    add_binary(0, &vreg_vreg, &shr);
    add_binary(1, &vreg, &shr);
    valid_instructions[op_num++] = shr;
    // - subn
    InstructionValidator subn = init_validator(1, OP_SUBN);
    add_binary(0, &vreg_vreg, &subn);
    valid_instructions[op_num++] = subn;
    // - shl
    InstructionValidator shl = init_validator(2, OP_SHL);
    add_binary(0, &vreg_vreg, &shl);
    add_binary(1, &vreg, &shl);
    valid_instructions[op_num++] = shl;
    // - rnd
    InstructionValidator rnd = init_validator(1, OP_RND);
    add_binary(0, &vreg_byte, &rnd);
    valid_instructions[op_num++] = rnd;
    // - drw
    InstructionValidator drw = init_validator(1, OP_DRW);
    add_binary(0, &vreg_vreg_nibble, &drw);
    valid_instructions[op_num++] = drw;
    // - skp
    InstructionValidator skp = init_validator(1, OP_SKP);
    add_binary(0, &vreg, &skp);
    valid_instructions[op_num++] = skp;
    // - sknp
    InstructionValidator sknp = init_validator(1, OP_SKNP);
    add_binary(0, &vreg, &sknp);
    valid_instructions[op_num++] = sknp;
}

/**
 * @brief Compare the instruction against a valid instruction argument suite.
 * I use this function to handle all special cases
 * 
 * @param inst Pointer to the instruction tested
 * @param argsuite The argsuite compared to
 * @return true if match
 * @return false if no match
 */
static bool compare_arguments(Instruction* inst, ArgSuite* argsuite) {
    
    // *** Check for special cases ***
    // JP V0 addr
    if(inst->opcode == OP_JP) {
        if(inst->args_count && inst->args[0].type == ARG_V_REGISTER) {
            if(inst->args[0].as.reg != REG_V0) return false;
        }
    }

    // *** General cases ***
    // Loop in argsuite arguments
    for(int i = 0; i < argsuite->arg_count; i++) {
        
        ArgType tested = inst->args[i].type;
        ArgType valid = argsuite->args[i]; 
        
        // If param type does not match
        if(tested != valid) {
            // If [address vs byte || nibble] => VALID
            if(valid == ARG_ADDRESS && (tested == ARG_BYTE || tested == ARG_NIBBLE)) continue;
            // If [byte vs nibble] => VALID
            else if(valid == ARG_BYTE && tested == ARG_NIBBLE) continue;
            // Else, bad argument provided
            return false;
        }
    }

    return true;
}

/**
 * @brief Checks the instruction provided
 * 
 * @param inst Instruction read by scanner
 * @return ArgSuite* The valid argsuite associated with instruction
 */
ArgSuite* check_instruction(Instruction* inst) {
    // Get the associated opcode
    bool op_found = false;
    int op_index = (int)inst->opcode;

    if(inst->opcode == valid_instructions[op_index].opcode) {
        // Check valids instructions
        for(int j = 0; j < valid_instructions[op_index].suites_count; j++) {
            
            ArgSuite* current_argsuite = valid_instructions[op_index].suites[j];
            if(compare_arguments(inst, current_argsuite)) return current_argsuite;
        }
    }

    return NULL;
}

static uint16_t get_address(Instruction* inst, int arg_index) {
    if(inst->args[arg_index].type == ARG_BYTE)   return (uint16_t)inst->args[arg_index].as.byte;
    if(inst->args[arg_index].type == ARG_NIBBLE) return (uint16_t)inst->args[arg_index].as.nibble;
    return inst->args[arg_index].as.address;
}

static uint8_t get_byte(Instruction* inst, int arg_index) {
    if(inst->args[arg_index].type == ARG_BYTE)   return inst->args[arg_index].as.byte;
    return inst->args[arg_index].as.nibble;
}




/**
 * @brief Encodes the valid instruction into CHIP8 binary
 * 
 * @param inst The valid instruction provided
 * @param argsuite The structure of arguments (not sure it is usefull)
 * @return uint16_t The encoded instruction
 */
uint16_t encode_instruction(Instruction* inst, ArgSuite* argsuite) {
    uint16_t bin;
    switch(inst->opcode) {
        case OP_CLS: return 0x00E0;

        case OP_RET: return 0x00EE;

        case OP_SYS: {
            return get_address(inst, 0) & 0x0FFF;
        }

        case OP_JP: {
            if(inst->args[0].type != ARG_V_REGISTER)    return 0x1000 | (get_address(inst, 0) & 0x0FFF);
            else                                        return 0xB000 | (get_address(inst, 1) & 0x0FFF);
        }

        case OP_CALL: return 0x2000 | (get_address(inst, 0) & 0x0FFF);

        case OP_SE: {
            if(argsuite->args[1] == ARG_BYTE) {
                return 0x3000 | ((uint8_t)inst->args[0].as.reg << 8) | get_byte(inst, 1);
            }
            else {
                return (0x5000 | ((uint8_t)inst->args[0].as.reg << 8) | (inst->args[1].as.reg << 4)) & 0xFFF0;
            }
        }

        case OP_SNE: {
            if(argsuite->args[1] == ARG_BYTE) {
                return 0x4000 | ((uint8_t)inst->args[0].as.reg << 8) | get_byte(inst, 1);
            }
            else {
                return (0x9000 | ((uint8_t)inst->args[0].as.reg << 8) | (inst->args[1].as.reg << 4)) & 0xFFF0;
            }
        }

        case OP_LD: {
            if(argsuite->args[1] == ARG_BYTE) {
                return 0x6000 | ((uint8_t)inst->args[0].as.reg << 8) | get_byte(inst, 1);
            }
            else if(argsuite->args[0] == ARG_V_REGISTER && argsuite->args[1] == ARG_V_REGISTER) {
                return (0x8000 | ((uint8_t)inst->args[0].as.reg << 8) | (inst->args[1].as.reg << 4)) & 0xFFF0;
            }
            else if (argsuite->args[0] == ARG_I_REGISTER) {
                switch (inst->args[0].as.reg) {
                    case REG_I: return 0xA000 | get_address(inst, 1);
                    case REG_F: return 0xF029 | ((inst->args[1].as.reg << 8) & 0x0F00);
                    case REG_B: return 0xF033 | ((inst->args[1].as.reg << 8) & 0x0F00);
                    default: error("SCANNER", -1, "Error in encoding");
                }
                if(inst->args[0].as.reg == REG_I) {
                    
                }
            }
            else if(argsuite->args[1] == ARG_DELAY_TIMER) {
                return 0xF007 | ((inst->args[0].as.reg << 8) & 0x0F00);
            }
            else if(argsuite->args[1] == ARG_KEY) {
                return 0xF00A | ((inst->args[0].as.reg << 8) & 0x0F00);
            }
            else if(argsuite->args[0] == ARG_DELAY_TIMER) {
                return 0xF015 | ((inst->args[1].as.reg << 8) & 0x0F00);
            }
            else if(argsuite->args[0] == ARG_SOUND_TIMER) {
                return 0xF018 | ((inst->args[1].as.reg << 8) & 0x0F00);
            }
            else if(argsuite->args[0] == ARG_I_INDIRECT) {
                return 0xF055 | ((inst->args[1].as.reg << 8) & 0x0F00);
            }
            else if(argsuite->args[1] == ARG_I_INDIRECT) {
                return 0xF065 | ((inst->args[0].as.reg << 8) & 0x0F00);
            }
        }

        case OP_ADD: {
            if (argsuite->args[0] == ARG_I_REGISTER) {
                return 0xF01E | ((inst->args[1].as.reg << 8) & 0x0F00);
            }
            else if(argsuite->args[1] == ARG_V_REGISTER) {
                return 0x8004 | ((inst->args[0].as.reg << 8) & 0x0F00) | ((inst->args[1].as.reg << 4) & 0x00F0);
            }
            else {
                return 0X7000 | ((inst->args[0].as.reg << 8) & 0x0F00) | (get_byte(inst, 1) & 0xFF);
            }
        }

        case OP_OR: return 0x8001 | ((inst->args[0].as.reg << 8) & 0x0F00) | ((inst->args[1].as.reg << 4) & 0x00F0);
        
        case OP_AND: return 0x8002 | ((inst->args[0].as.reg << 8) & 0x0F00) | ((inst->args[1].as.reg << 4) & 0x00F0);
        
        case OP_XOR: return 0x8003 | ((inst->args[0].as.reg << 8) & 0x0F00) | ((inst->args[1].as.reg << 4) & 0x00F0);
        
        case OP_SUB: return 0x8005 | ((inst->args[0].as.reg << 8) & 0x0F00) | ((inst->args[1].as.reg << 4) & 0x00F0);
        
        case OP_SHR: {
            if(argsuite->arg_count == 2) return 0x8006 | ((inst->args[0].as.reg << 8) & 0x0F00) | ((inst->args[1].as.reg << 4) & 0x00F0);
            else return 0x8006 | ((inst->args[0].as.reg << 8) & 0x0F00);
        }

        case OP_SUBN: return 0x8007 | ((inst->args[0].as.reg << 8) & 0x0F00) | ((inst->args[1].as.reg << 4) & 0x00F0);
        
        case OP_SHL: {
            if(argsuite->arg_count == 2) return 0x800E | ((inst->args[0].as.reg << 8) & 0x0F00) | ((inst->args[1].as.reg << 4) & 0x00F0);
            else return 0x800E | ((inst->args[0].as.reg << 8) & 0x0F00);
        }
        
        case OP_RND: return 0xC000 | ((inst->args[0].as.reg << 8) & 0x0F00) | (get_byte(inst, 1) & 0x00FF);
        
        case OP_DRW: return 0xD000 | ((inst->args[0].as.reg << 8) & 0x0F00) | ((inst->args[1].as.reg << 4) & 0x00F0) | (inst->args[2].as.nibble & 0xF);
        
        case OP_SKP: return 0xE09E | ((inst->args[0].as.reg << 8) & 0x0F00);
        
        case OP_SKNP: return 0xE0A1 | ((inst->args[0].as.reg << 8) & 0x0F00);
        
        // case OP_ERROR: printf("[OP_ERROR]"); break;
        default: return 0;
    }

    return bin;
}
 