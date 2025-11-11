#include "../inc/scanner.h"
#include <stdio.h>

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
    return c >= 'a' && c <= 'z';
}

static bool is_numeric(char c) {
    return c >= '0' && c <= '9';
}

static bool is_alphanumeric(char c) {
    return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

static bool is_invalid_char(char c) {
    return !(c == ' ' || c == '\n' || c == '\0' || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'));
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
            // case '\n':
            //     scanner.line++;
            //     advance();
            //     break;
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
        case 'e': return check_opcode(1, 3, "xit", OP_EXIT);
        case 'h': return check_opcode(1, 3, "igh", OP_HIGH);
        case 'j': return check_opcode(1, 1, "p", OP_JP);
        case 'l':
            switch(scanner.start[1]) {
                case 'd': return OP_LD;
                case 'o': return check_opcode(2, 1, "w", OP_LOW);
            }
            return OP_ERROR;
        case 'o': return check_opcode(1, 1, "r", OP_OR);
        case 'r':
            switch(scanner.start[1]) {
                case 'e': return check_opcode(2, 1, "t", OP_RET);
                case 'n': return check_opcode(2, 1, "d", OP_RND);
            }
            return OP_ERROR;
        case 'x': return check_opcode(1, 2, "or", OP_XOR);
        case 's': switch(scanner.start[1]) {
            case 'c': switch(scanner.start[2]) {
                case 'l': return OP_SCL;
                case 'd': return OP_SCD;
                case 'r': return OP_SCR;
            }
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


// static int number(bool is_hexa) {
//     if(is_hexa) return 0;
//     return -1;
// }



static Argument get_argument() {
    // printf("Entering get_argument with\n   start: '%c'\n   current:'%c'\n", *scanner.start, peek());
    Argument arg;
    arg.type = ARG_ERROR;
    if(is_alpha(*scanner.start)) {
        switch(*scanner.start) {
            case 'v': 
                if(is_numeric(scanner.start[1])) {
                    arg.type = ARG_REGISTER;
                    arg.as.reg = (Register)(scanner.start[1] - '0');
                    return arg;
                }
                else if(is_alpha(scanner.start[1]) && scanner.start[1] <= 'f') {
                    arg.type = ARG_REGISTER;
                    arg.as.reg = (Register)(scanner.start[1] - ('a'-10));
                    return arg;
                }
                
        }
    }
    return arg;
}
/**
 * @brief Reads a line and extract instruction if any.
 * 
 * @return Instruction 
 */
static void read_line() {
    int current_line = scanner.line;
    while(current_line == scanner.line && !is_at_end()) {
        
        // --- OPCODE --- 
        // Skip leading whitespace if any.
        skip_whitespace();

        // Exit if EOF
        if(is_at_end()) return;

        // Place scanner on token beginning
        scanner.start = scanner.current;

        // Check first opcode char.
        if(!is_alpha(peek())) {
            char str[68];
            sprintf(str, "The opcode must start with a lowcase alphabetic symbol. Got '%c'\n", peek());
            error("SCANNER", scanner.line, str);
        }
        // Get full opcode
        while(is_alpha(peek())) advance();
        Opcode op = get_opcode();
        if(op == OP_ERROR) error("SCANNER", scanner.line, "The opcode was not valid.");
        
        Instruction inst;
        inst.opcode = op;

        // --- ARGS ---
        skip_whitespace();
        // Place scanner on token beginning
        scanner.start = scanner.current;
        Argument arg = get_argument();
        printf("Opcode : %d\nArgument type: %d\nArgument value: %d\n\n", (int)op, (int)arg.type, (int)arg.as.reg);


        skip_whitespace();
    }

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
        while(is_alphanumeric(peek())) advance();
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
        printf("   [ARG]: Added argument type %d with value %d\n", (int)arg.type, (int)arg.as.reg);

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
    while(!is_at_end()) {
        // read_line();
        process_line();
    }
}

