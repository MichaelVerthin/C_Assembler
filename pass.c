#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "pass.h"
#include "asmbl.h"
#include "misc.h"
#include "line.h"
#include "globals.h"



int
first_pass(FILE *fptr, symbol_node **list)
{
    void *pfree;
    char *line = (char *)malloc(sizeof(char) * (LINE_LEN + 2));
    line_t *pLINE = (line_t *)malloc(sizeof(line_t));
    enum PARSE parse;
    int result = FALSE;

    pfree = line;

    if(!line || !pLINE)
        return -1;
    pLINE->head = *list; /* link the head of the symbol list to the line object */
    line_num = 0;   /* source code line number */
    IC = 0;  /* Instruction Counter */
    DC = 0;  /* Data Counter */

    /* Reading line by line of the file */
    while (fgets(line, LINE_LEN + 2, fptr) != NULL)
    {
        pLINE->line = trim_white(line);

        /* Clear newline characters from end of line with null terminator */
        if(line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        line_num++;

        /* Check if line is longer than LINE_LEN */
        if(strlen(line) > LINE_LEN)
        {
            char c;
            ERROR_MSG("Line Exceeds max line length.")
            while((c = fgetc(fptr)) != '\n' && c != '\0'); /* Skips to the end of the line to prevent unnecessary fgets calls */
            continue;
        }

        /* Check if line is a whitespace or a comment */
        if(skipable_line(pLINE->line))
            continue;


        /* main parsing function, the result is encoded directly to the instructions array */
        parse = parse_line(pLINE);
        result = encode(parse, pLINE, list);
    }

    /* Update the value of all the data symbols by IC+100 */
    LINE_FREE(pLINE);
    update_data(*list);

    /* free the last line_t object; all other dynamically objects will be cleaned on exit main */
    SAFE_FREE(pfree)
    
    return result;
}


int
second_pass(FILE *fptr, symbol_node **list, int oIC)
{
    void *pfree;
    char *line = (char *)malloc(sizeof(char)*LINE_LEN);
    line_t *pLINE = (line_t *)malloc(sizeof(line_t));

    pfree = line;
    /* Resetting the instruction counter and the line number */
    IC = 0;
    line_num = 0;
    
    if(!line || !pLINE)
    {
        SAFE_FREE(line)
        SAFE_FREE(pLINE)
        return FALSE;
    }

    /* rewind the file */
    rewind(fptr);

    while (fgets(line, LINE_LEN, fptr) != NULL)
    {
        pLINE->line = line;
        pLINE->line = clear_str(pLINE->line);

        /* Clear newline characters from end of line with null terminator */
        if(line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        line_num++;

        /* Skip whitespaces lines, comments, directives and externals statements */
        if(skipable_line(pLINE->line) || skip_lines_sec_pass(pLINE))
            continue;

        /* Entry statements is added to the linked list, the next line is scanned afterwards */
        if((is_entry(pLINE, *list)))
            continue;
    }

    /* Final encoding of the words in the instructions list */
    complete_encoding(*list, oIC); 

    get_entries(*list);

    /* Free linked list */
    free_list(*&list);
    SAFE_FREE(pfree)
    SAFE_FREE(pLINE)
    
    return 0;
}
