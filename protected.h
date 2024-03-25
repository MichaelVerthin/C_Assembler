#include <string.h>
#include "asmbl.h"
/**
 * Protected words that are forbidden to use.
 */
int is_safe(char *str)
{
    size_t i;
    for (i = 0; i < OPCODE_NUM; i++)
        if(!strcmp(str, )
}