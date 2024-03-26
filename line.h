#ifndef LINE_H
#define LINE_H

#include "asmbl.h"
#include "symbols.h"

typedef struct line_st
{
    char *line;
    char *label;
    int len;           /* number of words required */
    symbol_node *head; /* head of the linked list */
    symbol_t *parsed;
} line_t;

/* Three results that are available to parse_line() */
enum PARSE
{
    NOT_PARSED = 0,
    PARSED_MACRO = 1,
    PARSED_DIRECTIVE = 2,
    PARSED_INSTRUCTION = 4,
    PARSED_LABEL = 8,
    PARSED_FAILURE = -1,
    PARSED_SUCCESS = 0
};

/*
 * Main parsing function.
 * Checks for label, macro, directive and instruction properties of current line.
 */
int parse_line(line_t *pLINE);

/*
 * Encode the current line to the symbol list and the DC/IC arrays
 */
int encode(enum PARSE parse, line_t *pLINE, symbol_node **list);

/*
 * Special encoding for entry statements
 */
int entry_encode(char *entry, line_t *pLINE, symbol_node **list);

/*
 * Final encoding for the second pass
 */
void complete_encoding(symbol_node *list, int oIC);

/*
 * Line that is commented or that is consists of whitespaces only
 */
int skipable_line(char *);
int skip_lines_sec_pass(line_t *pLINE);

/*
 * Check if line is an entry statement and adds it to the linked list
 */
int is_entry(line_t *pLINE, symbol_node *list);
void get_entries(symbol_node *list);

#endif
