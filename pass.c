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
    int parse;

    IC = 0; /* Instruction Counter */
    static symbol_node list;

    /* Reading line by line of the file */
    while ((pLINE->line = fgets(line, LINE_LEN + 2, fptr)) != NULL)
    {
        ++IC;
        /* Check if line is longer than LINE_LEN */
        if (strlen(pLINE->line) > LINE_LEN)
        {
            ERROR_MSG("Line Exceeds max line length.")
            return EXCEEDS_MAX_LENGTH;
        }
        /* line is a whitespace or a comment - CONTINUE */
        if (skipable_line(pLINE->line))
            continue;
        /******************** PARSING LINE ********************/
        parse = parse_line(pLINE);  /**/
        if (parse)                  /**/
            return PARSING_FAILURE; /**/
        /******************************************************/
    }
    LINE_FREE(pLINE);
    destroy_list(list.next);
    return 0;
}

int second_pass(FILE *fptr)
{
    return 0;
}