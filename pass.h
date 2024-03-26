#ifndef PASS_H
#define PASS_H

#include "symbols.h"

int first_pass(FILE *pfile, symbol_node **symbol_list);           /* Performs the first pass of the assembler */
int second_pass(FILE *pfile, symbol_node **symbol_list, int oIC); /* Performs the second pass of the assembler */

#endif