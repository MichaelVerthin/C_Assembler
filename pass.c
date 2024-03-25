#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "pass.h"
#include "asmbl.h"
#include "macro.h"
#include "line.h"
int first_pass(FILE *fptr)
{
    char *line = (char *)malloc(sizeof(char) * LINE_LEN);
    line_t oLine;
    line_t *pLINE = &oLine;
    /* Reading line by line of the file, line is not a comment ';' */
    while (pLINE->line = fgets(line, LINE_LEN, fptr))
    {
        /* FIRST PASS */
        if (line[0] == ';' || isspace(line[0]))
            continue;
        parse_line(pLINE);
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

        fprintf(stdout, "%s", pLINE->line);
    }
    free(line);
    return EXIT_SUCCESS;
}

int second_pass(FILE *fptr)
{
    return EXIT_SUCCESS;
}