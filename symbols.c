#include <stdlib.h>
#include "misc.h"
#include "symbols.h"

symbol_t *init_macro();

symbol_t *
init_symbol(int type)
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
struct symbol_nd
{
    int type;
    symbol_t *next;
};
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