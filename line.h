#ifndef LINE_H
#define LINE_H
#define LINE_LEN 80  /* Line maximum length */
#define LABEL_LEN 31 /* Label maximum length */
#define MACRO_LEN 20 /* macro maximum length - length of ".define" */

#include "symbols.h"

typedef struct line_st
{
    char *line;
    char *label;
    symbol_t *parsed;
} line_t;

enum DIRECTIVE
{
    DIRECTIVE_DATA,
    DIRECTIVE_STRING,
    DIRECTIVE_ENTRY,
    DIRECTIVE_EXTERN
};

enum PARSE
{
    PARSED_MACRO,
    PARSED_DIRECTIVE,
    PARSED_INSTRUCTION,
    PARSED_FAILURE
};

int parse_line(line_t *);
int skipable_line(char *);

#endif