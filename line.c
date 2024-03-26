#include <string.h>
#include <ctype.h>
#include <assert.h> /* DEBUG */
#include "line.h"
#include "asmbl.h"
#include "misc.h"
#include "symbols.h"
#include "opcodes.h"
#include "globals.h"
#include "asmbl.h"

char *strsub(char *, size_t, char *); /* Extracts a substring from a given string */
int is_label(line_t *pLINE);          /* Checks if the line contains a label */
int is_macro(line_t *);               /* Checks if the line contains a macro */
int is_directive(line_t *);           /* Checks if the line contains a directive */
int is_instruction(line_t *);         /* Checks if the line contains an instruction */

/*
 *Checks if the line is a comment or consists only of whitespace
 */
int parse_line(line_t *pLINE)
{
    int res = NOT_PARSED;
    /* Labels */
    if (is_label(pLINE))
    {
        res |= PARSED_LABEL;
        /* Trim whitespaces */
        pLINE->line = clear_str(pLINE->line);
    }
    else
        pLINE->label = NULL;
    /* Macros */
    if (is_macro(pLINE))
        return (res | PARSED_MACRO);
    /* Directives */
    if (is_directive(pLINE))
        return (res | PARSED_DIRECTIVE);
    /* Instructions */
    if (is_instruction(pLINE))
        return (res | PARSED_INSTRUCTION);
    return res;
}

int is_directive(line_t *pLINE) /* Checks if a line represents a directive */
{
    char *type, *ch, *data, *tmp, *st;
    symbol_t *directive;

    st = tmp = (char *)malloc(sizeof(char) * LINE_LEN);
    strcpy(tmp, pLINE->line);

    ch = tmp;

    /* trim whitespaces from start */
    while (isspace(*ch))
        ch++;

    if (*(ch) != '.' || !tmp)
    {
        SAFE_FREE(tmp)
        return FALSE;
    }

    type = strtok(ch, " ");
    data = strtok(NULL, "\0");

    if (strcmp_hash(type, ".data")) /* .data */
    {
        int i = 0;
        int num;
        char *tmp, tmp_data[LINE_LEN];
        int property, value;
        strcpy(tmp_data, data);
        tmp = strtok(tmp_data, ",");
        if (!(directive = init_symbol(SYMBOL_DATA_NUMBERS)))
        {
            ERROR_MSG("Failed to allocate memory for symbol_t")
            SAFE_FREE(st)
            return FALSE;
        }

        while (tmp)
        {
            if (((num = is_num(tmp)) != _12BIT_MIN) || (search_list(pLINE->head, tmp, &value, &property) && property & SYMBOL_MACRO))
            {
                if (num != _12BIT_MIN)
                    directive->symbol->directive->nums[i++] = num; /* the number has parsed succesfully */
                else if (is_valid(value))
                    directive->symbol->directive->nums[i++] = value; /* found the macro value in the list */
                else
                    ERROR_MSG("Failed to build .data values")
            }
            tmp = clear_str(strtok(NULL, ","));
        }
        pLINE->len = i;
        pLINE->parsed = directive;
        SAFE_FREE(st)
        return TRUE;
    }

    else if (strcmp_hash(type, ".string")) /* .string */
    {
        if (data[0] == '"' && data[strlen(data) - 1] == '"')
        {
            if ((directive = init_symbol(SYMBOL_DATA_STRING)))
            {
                /* Clear quotation marks */
                data++;
                data[strlen(data) - 1] = '\0';

                /* Validates the characters in the string are of the ascii family */
                if (!is_string(data))
                    ERROR_MSG("The assembler doesn't support non-ascii characters")

                directive->symbol->directive->data = data;
                pLINE->len = strlen(data) + 1;
                pLINE->parsed = directive;
                SAFE_FREE(st)
                return TRUE;
            }
            else
            {
                ERROR_MSG("Failed to allocate memory for symbol_t")
                SAFE_FREE(st)
                return FALSE;
            }
        }
        else
        {
            ERROR_MSG("Data is not enclosed with quatation marks")
            SAFE_FREE(st)
            return FALSE;
        }
    }
    /***************************************************************************************/

    else if (strcmp_hash(type, ".entry"))
    {
        if ((directive = init_symbol(SYMBOL_DATA_STRING)))
            directive->symbol->directive->data = data;
        pLINE->parsed = directive;
        pLINE->parsed->type = SYMBOL_ENTRY;
        return TRUE;
    }

    else if (strcmp_hash(type, ".extern"))
    {
        int len = 0;
        trim_white(data);

        if (!is_name(data))
        {
            ERROR_MSG("Illegal symbol name")
            SAFE_FREE(st)
            return FALSE;
        }
        if ((directive = init_symbol(SYMBOL_DATA_STRING)))
        {
            strcpy(directive->symbol->directive->data, data);
            pLINE->parsed = directive;
            pLINE->parsed->type = SYMBOL_EXTERNAL;
            pLINE->len = len;
            SAFE_FREE(st)
            return TRUE;
        }
    }

    SAFE_FREE(st)

    return FALSE;
}

int is_instruction(line_t *pLINE) /* Checks if the line contains an instruction */
{
    char *tmp, *pst;
    char *operand, *line, *operand2;
    int code = ILLEGAL_INST;
    tmp = pst = (char *)malloc(sizeof(char) * LINE_LEN);
    if (tmp)
        strcpy(tmp, pLINE->line);
    else
        return FALSE;

    /* Extracts the opcode name */
    while (isspace(*tmp))
        tmp++;
    strtok(tmp, " \t");

    /* Validates the opcodes name */
    if ((code = is_opcode(tmp)) != ILLEGAL_INST)
    {
        size_t sz = 1; /* number of words, atleast one for the main instruction */
        int addmod, abs = ERROR, opvalue;
        char macro_name[MACRO_LEN];
        symbol_t *instruction = init_symbol(SYMBOL_CODE);
        int op2 = FALSE, op = check_operands(pLINE->line, code);
        if (op == ERROR || !instruction)
        {
            SAFE_FREE(pst)
            free_symbol(instruction);
            return FALSE;
        }

        line = clear_str(pLINE->line);
        if (strstr(line, ","))
            op2 = TRUE;
        strtok(line, "\t "); /* remove the opcode */
        operand = strtok(NULL, ",");
        if (op2)
        {
            operand2 = operand + strlen(operand) + 1;
            while (isspace(*operand2) || *operand2 == ',')
                operand2++;
        }
        /*
         * Source Operand parsing and analayzing
         */
        if (op > 1)
        {
            char name[MACRO_LEN];
            char index[MACRO_LEN];
            /* Get the correct address mode for the source operand */
            addmod = get_addmode(operand, code, SRC, &abs, &*macro_name);

            if (addmod == ERROR)
            {
                SAFE_FREE(pst)
                return FALSE;
            }

            switch (addmod)
            {
            case ADDMODE_3:
                /* Registry operand */
                instruction->symbol->instruction->source->addmod = ADDMODE_3;
                instruction->symbol->instruction->source->op->name = operand;
                instruction->symbol->instruction->source->are = ABSOLUTE;
                break;
            case ADDMODE_2:
                strcpy(name, strtok(operand, "["));
                strcpy(index, strtok(NULL, "]"));
                instruction->symbol->instruction->source->addmod = ADDMODE_2;
                instruction->symbol->instruction->source->op->name = name;
                if (is_num(index) != _12BIT_MIN)
                    instruction->symbol->instruction->source->index = atoi(index);
                else
                    instruction->symbol->instruction->source->str_index = index;
                instruction->symbol->instruction->source->are = _12BIT_MIN;
                break;
            case ADDMODE_1:
                instruction->symbol->instruction->source->op->name = operand;
                instruction->symbol->instruction->source->addmod = ADDMODE_1;
                instruction->symbol->instruction->source->are = _12BIT_MIN;
                break;
            case ADDMODE_0:
                instruction->symbol->instruction->source->addmod = ADDMODE_0;
                operand++; /* Skip the '#' character */
                if (((opvalue = is_num(operand)) != _12BIT_MIN))
                    instruction->symbol->instruction->source->op->value = opvalue;
                else
                    instruction->symbol->instruction->source->op->name = operand;
                instruction->symbol->instruction->source->are = ABSOLUTE;
                break;
            default:
                break;
            }

            sz += addmod_sz(addmod);
        }
        /*
         * Destination Operand parsing and analayzing
         */
        if (op > 0)
        {
            char *name = (char *)malloc(sizeof(char) * MACRO_LEN);
            char *index = (char *)malloc(sizeof(char) * MACRO_LEN);

            if (op == 1)
                operand2 = operand;
            /* Get the correct address mode for the destination operand */
            addmod = get_addmode(operand2, code, DST, &abs, &*macro_name);

            if (addmod == ERROR)
            {
                SAFE_FREE(pst)
                return FALSE;
            }

            switch (addmod)
            {
            case ADDMODE_3:
                /* Registry operand */
                instruction->symbol->instruction->destination->addmod = ADDMODE_3;
                instruction->symbol->instruction->destination->op->name = operand2;
                instruction->symbol->instruction->destination->are = ABSOLUTE;
                break;
            case ADDMODE_2:
                strcpy(name, strtok(operand2, "["));
                strcpy(index, strtok(NULL, "]"));
                instruction->symbol->instruction->destination->addmod = ADDMODE_2;
                instruction->symbol->instruction->destination->op->name = name;
                if (is_num(index) != _12BIT_MIN)
                    instruction->symbol->instruction->destination->index = atoi(index);
                else
                    instruction->symbol->instruction->destination->str_index = index;
                instruction->symbol->instruction->destination->are = 9999;
                break;
            case ADDMODE_1:
                instruction->symbol->instruction->destination->op->name = operand2;
                instruction->symbol->instruction->destination->addmod = ADDMODE_1;
                instruction->symbol->instruction->destination->are = 9999;
                break;
            case ADDMODE_0:
                instruction->symbol->instruction->destination->addmod = ADDMODE_0;
                operand2++; /* Skip the '#' character */
                if (((opvalue = is_num(operand2)) != _12BIT_MIN))
                    instruction->symbol->instruction->destination->op->value = opvalue;
                else
                    instruction->symbol->instruction->destination->op->name = operand2;
                instruction->symbol->instruction->destination->are = ABSOLUTE;

            default:
                break;
            }
            sz += addmod_sz(addmod);
        }

        if (instruction->symbol->instruction->destination->addmod == 0)
            instruction->symbol->instruction->destination->addmod = ADDMODE_0;
        if (instruction->symbol->instruction->source->addmod == 0)
            instruction->symbol->instruction->source->addmod = ADDMODE_0;

        /* number of words */
        pLINE->len = sz;
        pLINE->parsed = instruction;
        pLINE->parsed->type = SYMBOL_CODE;
        pLINE->parsed->symbol->instruction->opcode = code;

        SAFE_FREE(pst)

        return TRUE;
    }
    ERROR_MSG("Unrecognized instruction")
    return FALSE;
}

/**
 * Extractes .define macro to its name and data.
 * Constructs a new symbol_t struct
 * returns TRUE on success or FALSE on failure.
 */
int is_macro(line_t *pLINE)
{
    char tmp[LINE_LEN], *ch, *name, *data_s;
    int data;

    strcpy(tmp, pLINE->line);
    ch = tmp;

    /* trim whitespaces from start */
    while (isspace(*ch++))
        ;

    /* Extracte .define if exists */
    ch = strtok(ch - 1, " ");
    if (strcmp_hash(ch, ".define"))
    {
        symbol_t *macro = init_symbol(SYMBOL_MACRO);
        name = strtok(NULL, "=");
        if (!macro)
        {
            ERROR_MSG("Failed to allocate memory for symbol_t")
            return FALSE;
        }

        /* Extracting the name of the macro */
        if ((name = clear_str(name)) != NULL)
            strcpy(macro->symbol->macro->name, name);
        else
        {
            ERROR_MSG("Error Extracting macro name")
            free_symbol(macro);
            return FALSE;
        }

        /* Extracting the data of the macro */
        data_s = clear_str(strtok(NULL, "\0"));
        data = is_num(data_s);
        if (data != _12BIT_MIN)
            macro->symbol->macro->data = data;
        else
        {
            ERROR_MSG("Error extracting macro data")
            free_symbol(macro);
            return FALSE;
        }

        /* Pointing the line object to the macro object */
        pLINE->parsed = macro;
        return TRUE;
    }
    return FALSE;
}

/**
 * Check if line contains a label, returns the label name if exists.
 */
int is_label(line_t *pLINE)
{
    char *st, *ch, *label;
    ch = pLINE->line;
    while (isspace(*ch++))
        ;
    st = ch - 1;
    if (isalpha(*st))
    {
        /* Iterate over the characters of the line until reaching the end of the label title */
        while (isgraph(*ch) && *ch != ':')
            ch++;
        /* Reached the end of the label title */
        if (*ch == ':' && isspace(*(ch + 1)))
        {
            label = malloc(LABEL_LEN);
            strcpy(label, strtok(st, ":"));
            pLINE->line = strtok(NULL, "\0");

            /* Label is too long */
            if (ch - st > LABEL_LEN)
            {
                ERROR_MSG("Label is too long.")
                SAFE_FREE(label);
                return FALSE;
            }
            /* Label is a reserved word */
            if (is_reserved(label))
            {
                ERROR_MSG("Label name is a reserved word")
                SAFE_FREE(label);
                return FALSE;
            }
            pLINE->label = label;
            return TRUE;
        }
    }
    return FALSE;
}

/* Is the line completely blank? */
int is_whitespace(char *line)
{
    while (*line != '\0')
    {
        if (!isspace(*line++))
            return FALSE;
    }
    return TRUE;
}

/* Is the line a comment? */
int is_comment(char *line)
{
    return (*line == ';');
}

/* Is the line worth parsing at all? */
int skipable_line(char *line)
{
    return (is_comment(line) || is_whitespace(line));
}

/*
 * Allocates a new string which contains the characters
 * from pointer of index pos up to len characters,
 * adds the null terminator at the end of sub.
 * Returns the new substring; on failure returns NULL.
 * NOTE: The function uses dynamically allocated memory,
 * needs to be free()'ed with FREE_SAFE or LINE_FREE()
 */
char *
strsub(char *pos, size_t len, char *str)
{
    int c = 0;
    char *sub = (char *)malloc(sizeof(char) * len);
    if (!sub)
        return NULL;
    while (c < len)
    {
        sub[c] = *pos++;
        c++;
    }
    sub[c] = '\0';

    return sub;
}

/*
 * Check if the current line is .data .extern or .string
 */
int skip_lines_sec_pass(line_t *pLINE)
{
    char *ch, *st = ch = pLINE->line;
    if (isalpha(*st))
    {
        /* Iterate over the characters of the line until reaching the end of the label title */
        while (isalnum(*ch) && *ch != ':')
            ch++;
        /* Reached the end of the label title */
        if (*ch == ':')
            pLINE->line = clear_str(++ch);
    }
    if (strncmp(pLINE->line, ".data", strlen(".data") - 1) == 0)
        return TRUE;
    if (strncmp(pLINE->line, ".string", strlen(".string") - 1) == 0)
        return TRUE;
    if (strncmp(pLINE->line, ".extern", strlen(".extern") - 1) == 0)
        return TRUE;
    else
        return FALSE;
}

/*
 * Check if current line is .entry directive
 */
int is_entry(line_t *pLINE, symbol_node *list)
{
    char tmp[LINE_LEN];
    char *entry;
    strtok(tmp, " \t");
    if (strcmp_hash(tmp, ".entry"))
    {
        entry = strtok(NULL, " \t \0");
        if (is_name(entry))
            return entry_encode(entry, pLINE, &list);
    }
    return FALSE;
}
