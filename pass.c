#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "pass.h"
#include "asmbl.h"
#include "line.h"
#include "misc.h"

int first_pass(FILE *fptr)
{
    char *line = (char *)malloc(sizeof(char) * LINE_LEN);
    line_t *pLINE = (line_t *)malloc(sizeof(line_t));
    /* Reading line by line of the file, line is not whitespace or a comment */
    while ((pLINE->line = fgets(line, LINE_LEN + 1, fptr)) && !skipable_line(pLINE->line))
    {
        /* Check if line is longer than LINE_LEN */
        if (strlen(pLINE->line) > LINE_LEN)
        {
            fprintf(stderr, "Line Exceeds max line length %d.\n", LINE_LEN);
            continue;
        }
        parse_line(pLINE);
        if ((pLINE->label) != NULL)
            fprintf(stdout, "THIS IS LABEL: '%s'\n", pLINE->label);
    }
    SAFE_FREE(line)
    LINE_FREE(pLINE);
}

int second_pass(FILE *fptr)
{
    return EXIT_SUCCESS;
}