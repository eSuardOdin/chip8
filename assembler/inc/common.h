#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/// Beginning of most CHIP 8 programs
#define RAM_START 0x200

void error(const char* module, int line, const char* message);

#endif