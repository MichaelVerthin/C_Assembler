#ifndef OPCODES_H
#define OPCODES_H

#include "asmbl.h"
#include "misc.h"
#include "line.h"

/* Array index for opcode properties */
#define SRC 1
#define DST 2
#define OPERANDS_NUM 3

#define ERROR -1

/* A/R/E modifiers */
#define ABSOLUTE 0
#define EXTERNAL 1
#define RELOCATABLE 2
/******************/

int check_operands(char *line, unsigned inst);
int get_addmode(char *operand, unsigned code, int mode, int *abs, char *macro_name);
int addmod_sz(int mode);
int get_are(int mode);

/**
 * Enumeration of all the addressing modes available
 * on this assembler.
 * Using multiples of 2 to allow easy additions
 */
enum ADDRESS_MODES
{
    ADDMODE_NONE = 0,
    ADDMODE_0 = 1,
    ADDMODE_1 = 2,
    ADDMODE_1_LOCAL = 50,
    ADDMODE_1_EXTERNAL = 60,
    ADDMODE_2 = 4,
    ADDMODE_2_NUMBER = 20,
    ADDMODE_2_MACRO = 30,
    ADDMODE_3 = 8,
    ADDMODE_12 = ADDMODE_1 | ADDMODE_2,
    ADDMODE_13 = ADDMODE_1 | ADDMODE_3,
    ADDMODE_123 = ADDMODE_12 | ADDMODE_3,
    ADDMODE_ALL = ADDMODE_123 | ADDMODE_0
};

extern const int opcodes[OPCODE_NUM][4];


#endif