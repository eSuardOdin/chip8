#ifndef SCANNER_H
#define SCANNER_H
#include "common.h"
#include "instruction.h"

typedef struct {
    int line;
    int mem_addr;
    const char* start;
    const char* current;
    Instruction* instructions;
    int inst_cap;
    int inst_count;
} Scanner;



void init_scanner(const char* src);
void assemble(const char* src);

#endif