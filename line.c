#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "line.h"
line_t *parse_line(line_t *pLINE)
{
    char *parts[5] =
        {pLINE->first, pLINE->second, pLINE->third,
         pLINE->fourth, pLINE->fifth};
    int i = 0;
    char *ptr = pLINE->line;
    while (ptr)
    {
        if (isspace(*ptr) || *ptr == ',')
        {
            pLINE->first = pLINE->line;
        }
    }
    return pLINE;
}