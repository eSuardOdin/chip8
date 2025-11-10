#include "../inc/common.h"

void error(const char* module, int line, const char* message) {
    fprintf(stderr, "[%s] line %d: '%s'\n", module, line, message);
    exit(EXIT_FAILURE);
}