#ifndef SYMBOL_H
#define SYMBOL_H

enum SYMBOL
{
    SYMBOL_MACRO,
    SYMBOL_DATA,
    SYMBOL_CODE,
    SYMBOL_STRING
};

typedef struct macro_st
{
    char *name;
    int data;
} macro_t;

typedef struct directive_st
{
    char *directive;
} directive_t;

typedef struct instruction_st
{
    int type;
} instruction_t;

union symbol_un
{
    macro_t *macro;
    directive_t *directive;
    instruction_t *instruction;
};

/* SYMBOLS UNION */
typedef struct symbol_st
{
    union symbol_un *symbol;
    int type; /* according to enum SYMBOL */

} symbol_t;

symbol_t *init_macro();
symbol_t *init_symbol(int);
void destroy_macro(symbol_t *);

#endif