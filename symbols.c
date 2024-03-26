#include <stdlib.h>
#include <string.h>
#include "line.h"
#include "asmbl.h"
#include "misc.h"
#include "symbols.h"
#include "opcodes.h"
#include "globals.h"


symbol_t *init_macro();
symbol_t *init_data(enum SYMBOL type);
symbol_t *init_code();

symbol_t *
init_symbol(enum SYMBOL type)
{
    symbol_t *sym;
    switch (type)
    {
    case SYMBOL_MACRO:
        sym = init_macro();
        if(sym)
            return sym;
        break;
    case SYMBOL_DATA_STRING:
        sym = init_data(SYMBOL_DATA_STRING);
        if(sym)
            return sym;
        break;
    case SYMBOL_DATA_NUMBERS:
        sym = init_data(SYMBOL_DATA_NUMBERS);
            if(sym)
            return sym;
        break;
    case SYMBOL_CODE:
        sym = init_code();
        if(sym)
            return sym;
        break;
    default:
        break;
    }
    return NULL;
}

void
free_symbol(symbol_t *sym)
{
    if(!sym || !sym->symbol)
        return;
    switch (sym->type)
    {
    case SYMBOL_MACRO:
        SAFE_FREE(sym->symbol->macro->name)
        SAFE_FREE(sym->symbol->macro)
        SAFE_FREE(sym->symbol)
        SAFE_FREE(sym)
        return;
    case SYMBOL_DATA_STRING:
        SAFE_FREE(sym->symbol->directive)
        SAFE_FREE(sym->symbol)
        SAFE_FREE(sym)
        return;
    case SYMBOL_DATA_NUMBERS:
        SAFE_FREE(sym->symbol->directive->nums)
        SAFE_FREE(sym->symbol->directive)
        SAFE_FREE(sym->symbol)
        SAFE_FREE(sym)
        return;
    case SYMBOL_CODE:
        /* SAFE_FREE(sym->symbol->instruction->source->op->name) */
        /* SAFE_FREE(sym->symbol->instruction->destination->op->name) */
        if(sym->symbol->instruction->source->op)
            SAFE_FREE(sym->symbol->instruction->source->op)
        if(sym->symbol->instruction->destination->op)
            SAFE_FREE(sym->symbol->instruction->destination->op)
        if(sym->symbol->instruction->source)
            SAFE_FREE(sym->symbol->instruction->source)
        if(sym->symbol->instruction->destination)
            SAFE_FREE(sym->symbol->instruction->destination)
        if(sym->symbol->instruction)
            SAFE_FREE(sym->symbol->instruction)
        if(sym->symbol)
            SAFE_FREE(sym->symbol)
        if(sym)
            SAFE_FREE(sym)
        return;
    case SYMBOL_ENTRY:
        SAFE_FREE(sym->symbol->directive)
        SAFE_FREE(sym->symbol)
        SAFE_FREE(sym)
        return;
    default:
        return;
    }
}


/* Initialize symbol struct for the macro lines. */
symbol_t *
init_macro()
{
    symbol_t *macro_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    union symbol_un *symbol = (union symbol_un *)malloc(sizeof(union symbol_un));
    macro_t *macro = (macro_t *)malloc(sizeof(macro_t));
    char *name = (char *)malloc(sizeof(char)*MACRO_LEN);

    if (!macro_symbol || !symbol || !macro || !name)
        return NULL;
    
    macro->name = name;
    symbol->macro = macro;
    macro_symbol->symbol = symbol;

    macro_symbol->type = SYMBOL_MACRO;

    return macro_symbol;
}

/* Initialize symbol struct for the data lines. */
symbol_t *
init_data(enum SYMBOL type)
{
    symbol_t *data_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    union symbol_un *symbol = (union symbol_un *)malloc(sizeof(union symbol_un));
    directive_t *directive = (directive_t *)malloc(sizeof(directive_t));
    char *data = NULL;
    int *nums = NULL;

    if(data_symbol && symbol && directive)
    {

        /* Connecting the data objects together */
        symbol->directive = directive;
        data_symbol->symbol = symbol;

        /* for .string */
        if(type == SYMBOL_DATA_STRING)
        {
            data =(char *)malloc(sizeof(char)*LINE_LEN);
            if(data)
            {
                directive->data = data;
                data_symbol->type = SYMBOL_DATA_STRING;
                return data_symbol;
            }
            return NULL;
        }
        /* for .data */
        if (type == SYMBOL_DATA_NUMBERS)
        {
            nums = (int *)malloc(sizeof(int)*LINE_LEN);
            if(nums)
            {
                directive->nums = nums;
                data_symbol->type = SYMBOL_DATA_NUMBERS;
                return data_symbol;
            }
            return NULL;
        }
    }
    
    SAFE_FREE(data_symbol)
    SAFE_FREE(data)
    SAFE_FREE(directive)
    return NULL;
        
}


/* Initialize symbol struct for the code lines. */
symbol_t *
init_code()
{
    symbol_t *data_symbol = (symbol_t *)malloc(sizeof(symbol_t));
    union symbol_un *symbol = (union symbol_un *)malloc(sizeof(union symbol_un));
    struct operand *source = (struct operand *)malloc(sizeof(struct operand));
    struct operand *destination = (struct operand *)malloc(sizeof(struct operand));
    union op *dst_op = (union op *)malloc(sizeof(union op *));
    union op *src_op = (union op *)malloc(sizeof(union op *));
    instruction_t *code = (instruction_t *)malloc(sizeof(instruction_t));

    if(!data_symbol || !symbol || !code || !source || !destination || !src_op || !dst_op)
    {
        SAFE_FREE(data_symbol)
        SAFE_FREE(symbol)
        SAFE_FREE(code)
        SAFE_FREE(source)
        SAFE_FREE(destination)
        SAFE_FREE(dst_op)
        SAFE_FREE(src_op)
        return NULL;
    }

    src_op->name = NULL;
    dst_op->name = NULL;
    destination->op = dst_op;
    source->op = src_op;
    code->source = source;
    code->destination = destination;
    symbol->instruction = code;
    data_symbol->symbol = symbol;
    
    return data_symbol;
}


symbol_node *
next_node(symbol_node **list, char *name, int value, enum SYMBOL property)
{
    char *node_name;
    symbol_node *tmp, *node;

    node = (symbol_node *)calloc(sizeof(symbol_node), 1);
    tmp = *list;

    node_name = (char *)malloc(sizeof(char)*strlen(name));

    if(!node || !node_name)
        return NULL;

    strcpy(node_name, name);
    node->name = node_name;
    node->value = value;
    node->property = property;
    node->next = NULL;

    if(!(*list))
    {
        (*list)->name = node_name;
        (*list)->value = value;
        (*list)->property = property;
        (*list)->next = NULL;
        SAFE_FREE(node)
        return *list;
    }
    

    /* The list is not empty; get the last node andn set it */
    while(tmp)
    {
        /* reached the end of the linked list */
        if(!tmp->next)
        {
            tmp->next = node; /* setting the new node */
            break;
        }
        else
            tmp = tmp->next; /* proceeds to the next node */
    }        

    return *list;
}

int
search_list(const symbol_node *list, char *name, int *value, int *property)
{
    char s[LINE_LEN];
    symbol_node **tmp = (symbol_node **) &list;
    if(!name)
        return ERROR;
    strcpy(s, name);

    while((*tmp))
    {
        if(strcmp_hash((*tmp)->name, s) && (*tmp)->property  & ~(SYMBOL_ENTRY | SYMBOL_EXTERNAL))
        {
            if(property)
                *property = (*tmp)->property;
            if(value)
                *value = (*tmp)->value;
            return (*tmp)->value;
        }
        (*tmp) = (*tmp)->next;
    }
    return ERROR;
}

int
search_list_property(const symbol_node *list, char *name, int *value, int property)
{
    char s[LINE_LEN];
    symbol_node **tmp = (symbol_node **) &list;
    if(!name)
        return ERROR;
    strcpy(s, name);

    while((*tmp))
    {
        if(strcmp_hash((*tmp)->name, s) && ((*tmp)->property == property))
        {
            if(value)
                *value = (*tmp)->value;
            return (*tmp)->value;
        }
        (*tmp) = (*tmp)->next;
    }
    return ERROR;
}

void 
free_list(symbol_node **list)
{
    symbol_node **tmp = list;
    while(*tmp)
    {
        
        *list = (*list)->next;
        if((*tmp)->name)
            SAFE_FREE((*tmp)->name)
        if(*tmp)
            SAFE_FREE(*tmp)
        *tmp = *list;
    }
}

void
update_data(symbol_node *list)
{
    symbol_node *tmp = list;
    while (tmp)
    {
        if(tmp->property & (SYMBOL_DATA_NUMBERS | SYMBOL_DATA_STRING))
            tmp->value += IC + 100;
        tmp = tmp->next;
    }
    
}