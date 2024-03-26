#ifndef PASS_H
#define PASS_H

#include "symbols.h"

int first_pass(FILE * pfile, symbol_node **symbol_list);
int second_pass(FILE * pfile, symbol_node **symbol_list, int oIC);

#endif