#include <stdlib.h>
#include "misc.h"
#include "symbols.h"

symbol_t *init_macro();

symbol_t *
init_symbol(enum SYMBOL type)
{
    symbol_t *sym;
    switch (type)
    {
    case SYMBOL_MACRO:
        sym = init_macro();
        if (!sym)
            return sym;
        break;
    case SYMBOL_DATA:
        sym = NULL;
        if (sym)
            return sym;
        break;
    default:
        break;
    }
    return NULL;
}
/**
 * Initialize symbol struct for the macro lines.
 */
symbol_t *
init_macro()
{
    symbol_t *macro_union = (symbol_t *)malloc(sizeof(symbol_t));
    union symbol_un *symbol = (union symbol_un *)malloc(sizeof(union symbol_un));
    macro_t *macro = (macro_t *)malloc(sizeof(macro_t));

    symbol->macro = macro;
    macro_union->symbol = symbol;

    if (!macro_union || !symbol || !macro)
        return NULL;

    return macro_union;
}

/**
 * Destroys and frees all dynamically allocated objects of
 * macro union struct.
 */
void destroy_macro(symbol_t *macro)
{
    if (macro)
    {
        if (macro->symbol)
        {
            if (macro->symbol->macro)
                SAFE_FREE(macro->symbol->macro)
            SAFE_FREE(macro->symbol)
        }
        SAFE_FREE(macro)
    }
}
/**
 * Creates a new node of symbol_t for the linked list,
 * with the properties of symbol and its type.
 * returns the list with the new node connected to its tail.
 * clean with destroy_list();
 */
symbol_node *
new_node(symbol_node *node, symbol_t *symbol, enum SYMBOL type)
{
    symbol_node *tmp, *new;
    tmp = node;
    /* Iterating through the linked list */
    while (tmp->next)
        tmp->data = tmp->next;

    new = (symbol_node *)malloc(sizeof(symbol_node));
    if (!new)
        return NULL;

    new->type = type;
    switch (type)
    {
    case SYMBOL_MACRO:
        new->data = symbol->symbol->macro;
        break;

    default:
        break;
    }

    return new;
}

void destroy_list(symbol_node *list)
{
    symbol_node *tmp = list;
    while (tmp->next)
    {
        list = list->next;
        SAFE_FREE(tmp)
        tmp = list;
    }
    return;
}