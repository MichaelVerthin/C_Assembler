#include <stdlib.h>
#include "asmbl.h"
#include "macro.c"

int first_pass(FILE *fptr)
{
    char *line = (char *)malloc(sizeof(char) * LINE_LEN);

    /* Reading line by line of the file, line is not a comment ';' */
    while (fgets(line, LINE_LEN, fptr) && line[0] != ';')
    {
        /* FIRST PASS */

        /* Is MACRO? */
        /**
         * TODO - Configure Macro checker
         */
        /* Is Data Holder? such as array or variable? */
        /**
         * TODO - Configure array parser
         */
        /* .extern or .entry ? */
        /**
         * TODO - Configure .extern/.entry
         */

        fprintf(stdout, "%s", line);
    }
    return EXIT_SUCCESS;
}

int second_pass(FILE *fptr)
{
    return EXIT_SUCCESS;
}