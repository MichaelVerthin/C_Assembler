#include <stdlib.h>
#include "asmbl.h"
#include "line.h"

int ismacro(LINER *line)
{
    if (!strcmp(line->first, ".define"))
        return TRUE;
    return 1;
}