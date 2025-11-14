#include "../inc/scanner.h"
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

Scanner scanner;

/**
 * @brief Returns the current char and advances to the next one.
 * 
 * @return char The current char.
 */
static char advance() {
    return *scanner.current++;
}

/**
 * @brief Just checks if scanned file is at end.
 * 
 * @return true If at end.
 * @return false If not at end.
 */
static bool is_at_end() {
    return *scanner.current == '\0';
}


/**
 * @brief Returns the char `distance` after current.
 * Do not consume the char.
 * @param distance The distance from current to peek.
 * 
 * @return `char` The char peeked.
 */
static char peek() {
    return *scanner.current;
}

static char peekNext() {
    return is_at_end() ? '\0' : scanner.current[1];
}


/**
 * @brief To check if a char is alphabetic.
 * 
 * @param c The char to check.
 * @return true if char is alpha.
 * @return false if char is not alpha.
 */
static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool is_numeric(char c) {
    return c >= '0' && c <= '9';
}

static bool is_arg_symbol(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '[' || c == ']';
}

static bool is_invalid_char(char c) {
    return !(c == ' ' || c == '\n' || c == '\0' || c == '[' || c == ']' || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'));
}

static void skip_whitespace() {
    while(1) {
        char c = peek();
        switch(c) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            // Check if comment
            case '#':
                while(peek() != '\n' && !is_at_end()) advance();
                return;
            default:
                return;
        }
    }
}


static Opcode check_opcode(int start, int lenght, const char* rest, Opcode op) {
    if(scanner.current - scanner.start == start + lenght && memcmp(scanner.start + start, rest, lenght) == 0) {
        return op;
    }
    return OP_ERROR;
}

static Opcode get_opcode() {
    switch(*scanner.start) {
        case 'a':
            switch(scanner.start[1]) {
                case 'd': return check_opcode(2, 1, "d", OP_ADD);
                case 'n': return check_opcode(2, 1, "d", OP_AND);
            }
            return OP_ERROR;
        case 'c':
            switch(scanner.start[1]) {
                case 'l': return check_opcode(2, 1, "s", OP_CLS);
                case 'a': return check_opcode(2, 2, "ll", OP_CALL);
            }
            return OP_ERROR;
        case 'd': return check_opcode(1, 2, "rw", OP_DRW);
        case 'j': return check_opcode(1, 1, "p", OP_JP);
        case 'l': return check_opcode(1, 1, "d", OP_LD);
        case 'o': return check_opcode(1, 1, "r", OP_OR);
        case 'r':
            switch(scanner.start[1]) {
                case 'e': return check_opcode(2, 1, "t", OP_RET);
                case 'n': return check_opcode(2, 1, "d", OP_RND);
            }
            return OP_ERROR;
        case 'x': return check_opcode(1, 2, "or", OP_XOR);
        case 's': switch(scanner.start[1]) {
            // case 'c': switch(scanner.start[2]) {
            //     case 'l': return OP_SCL;
            //     case 'd': return OP_SCD;
            //     case 'r': return OP_SCR;
            // }
                return OP_ERROR;
            case 'e': return OP_SE;
            case 'h': switch(scanner.start[2]) {
                case 'l': return OP_SHL;
                case 'r': return OP_SHR;
            }
                return OP_ERROR;
            case 'k': switch(scanner.start[2]) {
                case 'n': return check_opcode(3, 1, "p", OP_SKNP);
                case 'p': return OP_SKP;
            }
                return OP_ERROR;
            case 'n': return check_opcode(2, 1, "e", OP_SNE);
            case 'u': switch(scanner.start[2]) {
                case 'b':
                    Opcode is_subn = check_opcode(3, 1, "n", OP_SUBN);
                    if(is_subn != OP_ERROR) return is_subn;
                    return OP_SUB;
                }
                return OP_ERROR;
            case 'y': return check_opcode(2, 1, "s", OP_SYS);
        }

    }
    return OP_ERROR;
}

/**
 * @brief Returns a number argument. If an argument type is ARG_NIBBLE, it can
 * be used for a ARG_BYTE or ARG_ADDRESS but an ARG_BYTE cannot be used for an
 * ARG_NIBBLE
 * 
 * @return Argument 
 */
static void process_number(Argument* arg) {
    char nbstr[((int)(scanner.current - scanner.start)) + 1];
    nbstr[(int)(scanner.current - scanner.start)] = '\0';
    char* endptr;
    strncpy(nbstr, scanner.start, (int)(scanner.current - scanner.start));

    long nb = strtol(nbstr, &endptr, 0);
    
    if(*endptr != '\0') {
        printf("--> '%c'\n", *endptr);
        if(((int)(scanner.current - scanner.start) < 32)) {
                char arg_err[((int)(scanner.current - scanner.start)) + 1];
                strncpy(arg_err, scanner.start, (int)(scanner.current - scanner.start));
                arg_err[(int)(scanner.current - scanner.start)] = '\0';
                char err_msg[64];
                sprintf(err_msg, "The argument '%s' is not a valid number.", arg_err);
                error("SCANNER", scanner.line, err_msg);
            } else {
                char err_msg[64];
                sprintf(err_msg, "The argument n°%d is not a valid number.", scanner.instructions->args_count);
                error("SCANNER", scanner.line, err_msg);
            }
    }

    // Assign type and value
    if(nb <= 0xF) {
        arg->type = ARG_NIBBLE;
        arg->as.nibble = (uint8_t)nb;
    } else if(nb <= 0xFF) {
        arg->type = ARG_BYTE;
        arg->as.byte = (uint8_t)nb;
    } else if(nb <= 0xFFF) {
        arg->type = ARG_ADDRESS;
        arg->as.address = (uint16_t)nb;
    } else {
        char err_msg[64];
        sprintf(err_msg, "The argument '%ld' is too wide for an argument.", nb);
        error("SCANNER", scanner.line, err_msg);
    }
}

static void process_register(Argument* arg) {

    // Check if 'v' register
    if(*scanner.start == 'v') {
        // Check for invalid register argument
        if((int)(scanner.current - scanner.start) > 2) {
            
            if(((int)(scanner.current - scanner.start) < 32)) {
                char arg_err[((int)(scanner.current - scanner.start)) + 1];
                strncpy(arg_err, scanner.start, (int)(scanner.current - scanner.start));
                arg_err[(int)(scanner.current - scanner.start)] = '\0';
                char err_msg[64];
                sprintf(err_msg, "The argument '%s' is not valid.", arg_err);
                error("SCANNER", scanner.line, err_msg);
            } else {
                char err_msg[64];
                sprintf(err_msg, "The argument n°%d is not valid.", scanner.instructions->args_count);
                error("SCANNER", scanner.line, err_msg);
            }
        }
        // Valid V register
        arg->type = ARG_V_REGISTER;
        if(is_numeric(scanner.start[1])) {
                arg->as.reg = (Register)(scanner.start[1] - '0');
            }
        else if(is_alpha(scanner.start[1]) && scanner.start[1] <= 'f') {
            arg->as.reg = (Register)(scanner.start[1] - ('a'-10));
        }
                
        
    }
    // Check if 'k' (key input)
    else if(*scanner.start == 'k') {
        if((int)(scanner.current - scanner.start) != 1) {
            
            if(((int)(scanner.current - scanner.start) < 55)) {
                char arg_err[((int)(scanner.current - scanner.start)) + 1];
                strncpy(arg_err, scanner.start, (int)(scanner.current - scanner.start));
                arg_err[(int)(scanner.current - scanner.start)] = '\0';
                char err_msg[64];
                sprintf(err_msg, "The argument '%s' is not valid.", arg_err);
                error("SCANNER", scanner.line, err_msg);
            } else {
                char err_msg[64];
                sprintf(err_msg, "The argument n°%d is not valid.", scanner.instructions->args_count);
                error("SCANNER", scanner.line, err_msg);
            }
        }
        arg->type = ARG_KEY;
    }
    // Check if 'i' register
    else if(*scanner.start == 'i') {
        if((int)(scanner.current - scanner.start) != 1) {
            
            if(((int)(scanner.current - scanner.start) < 55)) {
                char arg_err[((int)(scanner.current - scanner.start)) + 1];
                strncpy(arg_err, scanner.start, (int)(scanner.current - scanner.start));
                arg_err[(int)(scanner.current - scanner.start)] = '\0';
                char err_msg[64];
                sprintf(err_msg, "The argument '%s' is not valid.", arg_err);
                error("SCANNER", scanner.line, err_msg);
            } else {
                char err_msg[64];
                sprintf(err_msg, "The argument n°%d is not valid.", scanner.instructions->args_count);
                error("SCANNER", scanner.line, err_msg);
            }
        }
        arg->type = ARG_I_REGISTER;
        arg->as.reg = REG_I;
    }
    else if(*scanner.start == 'f') {
        if((int)(scanner.current - scanner.start) != 1) {
            
            if(((int)(scanner.current - scanner.start) < 55)) {
                char arg_err[((int)(scanner.current - scanner.start)) + 1];
                strncpy(arg_err, scanner.start, (int)(scanner.current - scanner.start));
                arg_err[(int)(scanner.current - scanner.start)] = '\0';
                char err_msg[64];
                sprintf(err_msg, "The argument '%s' is not valid.", arg_err);
                error("SCANNER", scanner.line, err_msg);
            } else {
                char err_msg[64];
                sprintf(err_msg, "The argument n°%d is not valid.", scanner.instructions->args_count);
                error("SCANNER", scanner.line, err_msg);
            }
        }
        arg->type = ARG_I_REGISTER;
        arg->as.reg = REG_F;
    }
    else if(*scanner.start == 'b') {
        if((int)(scanner.current - scanner.start) != 1) {
            
            if(((int)(scanner.current - scanner.start) < 55)) {
                char arg_err[((int)(scanner.current - scanner.start)) + 1];
                strncpy(arg_err, scanner.start, (int)(scanner.current - scanner.start));
                arg_err[(int)(scanner.current - scanner.start)] = '\0';
                char err_msg[64];
                sprintf(err_msg, "The argument '%s' is not valid.", arg_err);
                error("SCANNER", scanner.line, err_msg);
            } else {
                char err_msg[64];
                sprintf(err_msg, "The argument n°%d is not valid.", scanner.instructions->args_count);
                error("SCANNER", scanner.line, err_msg);
            }
        }
        arg->type = ARG_I_REGISTER;
        arg->as.reg = REG_B;
    }
    // Check if '[i]' register
    else if(*scanner.start == '[') {
        if((int)(scanner.current - scanner.start) != 3) {
            
            if(((int)(scanner.current - scanner.start) < 55)) {
                char arg_err[((int)(scanner.current - scanner.start)) + 1];
                strncpy(arg_err, scanner.start, (int)(scanner.current - scanner.start));
                arg_err[(int)(scanner.current - scanner.start)] = '\0';
                char err_msg[64];
                sprintf(err_msg, "The argument '%s' is not valid.", arg_err);
                error("SCANNER", scanner.line, err_msg);
            } else {
                char err_msg[64];
                sprintf(err_msg, "The argument n°%d is not valid.", scanner.instructions->args_count);
                error("SCANNER", scanner.line, err_msg);
            }
        }
        arg->type = ARG_I_INDIRECT;
        arg->as.reg = REG_I_INDIRECTION;
    } 
    // Process DT and ST registers
    else {
        if((int)(scanner.current - scanner.start) != 2) {
            
            if(((int)(scanner.current - scanner.start) < 32)) {
                char arg_err[((int)(scanner.current - scanner.start)) + 1];
                strncpy(arg_err, scanner.start, (int)(scanner.current - scanner.start));
                arg_err[(int)(scanner.current - scanner.start)] = '\0';
                char err_msg[64];
                sprintf(err_msg, "The argument '%s' is not valid.", arg_err);
                error("SCANNER", scanner.line, err_msg);
            } else {
                char err_msg[64];
                sprintf(err_msg, "The argument n°%d is not valid.", scanner.instructions->args_count);
                error("SCANNER", scanner.line, err_msg);
            }
        }
        if(*scanner.start == 'd' && scanner.start[1] == 't') {
            arg->type = ARG_TIME_REGISTER;
            arg->as.reg = REG_DELAY_TIMER; 
        } else if(*scanner.start == 's' && scanner.start[1] == 't') {
            arg->type = ARG_TIME_REGISTER;
            arg->as.reg = REG_SOUND_TIMER; 
        }
    }
}

static Argument get_argument() {
    // printf("Entering get_argument with\n   start: '%c'\n   current:'%c'\n", *scanner.start, peek());
    Argument arg;
    arg.type = ARG_ERROR;
    if(is_alpha(*scanner.start) || *scanner.start == '[') {
        process_register(&arg);
    } else if (is_numeric(*scanner.start)) {
        process_number(&arg);
    }
    return arg;
}


static void process_line() {
    printf("[DEBUG] process_line() : line %d\n", scanner.line);
    // Skip trailing whitespace if any
    skip_whitespace();
    // Return if empty line
    if(peek() == '\n' || is_at_end()) {
        if(*scanner.current == '\n') {
            scanner.line++;
            advance();
        }
        return;
    }
    
    scanner.start = scanner.current;
    // Opcode processing
    while(is_alpha(peek())) advance();
    if(is_invalid_char(peek())) {
        char err_str[64];
        sprintf(err_str, "The character '%c' is not valid.", *scanner.current);
        error("SCANNER", scanner.line, err_str);
    }

    Opcode op = get_opcode();
    if(op == OP_ERROR) error("SCANNER", scanner.line, "The opcode was not valid.");

    printf("[OPCODE] %d\n", op);
    // Instruction instance
    Instruction instruction;
    init_instruction(&instruction);
    instruction.opcode = op;

    // Arguments or newline
    while(1) {
        skip_whitespace();
        if(peek() == '\n' || is_at_end()) {
            if(*scanner.current == '\n') {
                scanner.line++;
                advance();
            }
            return;
        }


        scanner.start = scanner.current;
        while(is_arg_symbol(peek())) advance();
        // Check if invalid char
        if(is_invalid_char(peek())) {
            char err_str[64];
            sprintf(err_str, "The character '%c' is not valid.", *scanner.current);
            error("SCANNER", scanner.line, err_str);
        }
        Argument arg = get_argument();
        if(arg.type == ARG_ERROR){
            char err_str[64];
            sprintf(err_str, "The argument n°%d was not valid.", instruction.args_count + 1);
            error("SCANNER", scanner.line, err_str);
        }
        add_argument(&instruction, &arg);


        // Check instruction 
        if(check_instruction(&instruction) == NULL) {
            error("SCANNER", scanner.line, "Instruction not valid.");
        } else {
            printf("[INSTRUCTION VALIDATED]\n\n");
        }
        // Debug
        switch(arg.type) {
            case ARG_V_REGISTER:
                printf("   [ARG]: Added argument type ARG_V_REGISTER with value %d\n", (int)arg.as.reg);
                break;
            case ARG_I_REGISTER:
                printf("   [ARG]: Added argument type ARG_I_REGISTER with value %d\n", (int)arg.as.reg);
                break;
            case ARG_I_INDIRECT:
                printf("   [ARG]: Added argument type ARG_I_INDIRECT with value %d\n", (int)arg.as.reg);
                break;
            case ARG_TIME_REGISTER:
                printf("   [ARG]: Added argument type ARG_TIME_REGISTER with value %d\n", (int)arg.as.reg);
                break;
            case ARG_ADDRESS:
                printf("   [ARG]: Added argument type ARG_ADDRESS with value %d\n", (int)arg.as.address);
                break;
            case ARG_BYTE:
                printf("   [ARG]: Added argument type ARG_BYTE with value %d\n", (int)arg.as.nibble);
                break;
            case ARG_NIBBLE:
                printf("   [ARG]: Added argument type ARG_NIBBLE with value %d\n", (int)arg.as.nibble);
                break;
            case ARG_KEY:
                printf("   [ARG]: Added argument type ARG_KEY\n");
                break;
            default:    return;
        }
    }
    if(*scanner.current == '\n') {
        scanner.line++;
        advance();
    }
}



/**
 * @brief Create base state of the scanner.
 * 
 * @param src The assembly file we read from.
 */
void init_scanner(const char* src) {
    scanner.start = src;
    scanner.current = src;
    scanner.line = 1;
    scanner.mem_addr = RAM_START;
    scanner.inst_cap = 0;
    scanner.inst_count = 0;
    scanner.instructions = NULL;
}



void assemble(const char* src) {
    init_scanner(src);
    // Create template instruction to check
    init_valid_instructions();
    while(!is_at_end()) {
        // read_line();
        process_line();
    }
}

