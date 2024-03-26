#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Included header files for the assembly process */
#include "asmbl.h"   /* Header for assembly language related functions */
#include "symbols.h" /* Header for symbol table management functions */
#include "misc.h"    /* Header for miscellaneous utility functions */
#include "files.h"   /* Header for functions related to file operations */
#include "line.h"    /* Header for line parsing related functions */
#include "opcodes.h" /* Header for opcode definitions and functions */
#include "globals.h" /* Header for global variables used throughout assembly */

/* Function prototypes for symbol table and instruction encoding functions */
void build_absolute_word(symbol_node *list, char *name, int opvalue, int bSRC, int index);
void build_relocatable_word(symbol_node *list, char *name, int index);
void build_external_word(symbol_node *list, char *name, int index);
int reg_address(char *reg, int src);
void encode_inst(line_t *pLINE, symbol_node **list);

/**
 * Function to add a new element to the symbol linked list
 *
 * This function takes the parsed line information (`pLINE`), symbol list (`list`),
 * and parse type (`parse`) as arguments. It then performs the following actions based on the parse type:
 *   - PARSED_MACRO: Adds a new macro node to the list if the name isn't already present.
 *   - PARSED_DIRECTIVE: Handles directives like .ENTRY and .EXTERNAL, updating symbol table accordingly.
 *       - For .ENTRY, adds a symbol with the directive data and property SYMBOL_ENTRY.
 *       - For .EXTERNAL, adds a symbol with the directive data and property SYMBOL_EXTERNAL.
 *       - For a data directive (no specific directive type mentioned), adds a symbol with an empty string
 *         as name, property SYMBOL_DATA_STRING, and updates the data array with the directive data.
 *   - PARSED_INSTRUCTION: Calls the `encode_inst` function to handle instruction encoding.
 *   - PARSED_LABEL: Adds a new label node to the list if the name isn't already present, updating
 *     instruction counter (IC) and symbol property (SYMBOL_CODE).
 *   - Combinations of PARSED_LABEL with other types (like PARSED_MACRO or PARSED_DIRECTIVE):
     - Emit error messages for invalid label usage (e.g., label on a macro line).
 *   - Default: No action.
 *
 * The function returns 0 on success or 1 on any error or warning.
 */
int encode(enum PARSE parse, line_t *pLINE, symbol_node **list)
{
    int i, ERR_FLAG = FALSE;
    int value, property;
    char tmp[LABEL_LEN];

    switch ((int)parse)
    {
    case PARSED_MACRO:
        /* Check if a macro with the same name already exists */
        if (search_list(*list, pLINE->parsed->symbol->macro->name, NULL, NULL) != ERROR)
        {
            ERROR_MSG("Macro name was already set");
            ERR_FLAG = TRUE;
        }
        else
        {
            /* Add a new macro node to the list */
            next_node(*&list, pLINE->parsed->symbol->macro->name, pLINE->parsed->symbol->macro->data, SYMBOL_MACRO);
        }
        break;

    case PARSED_DIRECTIVE:
        if (pLINE->parsed->type == SYMBOL_ENTRY)
        {
            /* Add a symbol for the .ENTRY directive */
            next_node(*&list, pLINE->parsed->symbol->directive->data, IC + 100, SYMBOL_ENTRY);
            break;
        }
        if (pLINE->parsed->type == SYMBOL_EXTERNAL)
        {
            /* Add a symbol for the .EXTERNAL directive */
            next_node(*&list, pLINE->parsed->symbol->directive->data, IC + 100, SYMBOL_EXTERNAL);
            break;
        }
        /* Handle data directive (no specific directive type mentioned) */
        next_node(*&list, EMPTY_STRING, DC, SYMBOL_DATA_STRING);
        for (i = 0; i < pLINE->len; i++)
        {
            data_arr[DC + i].reg = pLINE->parsed->symbol->directive->data[i];
        }
        break;

    case PARSED_INSTRUCTION:
        /* Delegate instruction encoding to the encode_inst function */
        encode_inst(pLINE, list);
        break;

    case PARSED_LABEL:
        /* Check for existing label with the same name (excluding SYMBOL_ENTRY) */
        if ((search_list(*list, pLINE->label, &value, &property) != ERROR) && property != SYMBOL_ENTRY)
        {
            ERROR_MSG("Label name was already set");
            ERR_FLAG = TRUE;
            break;
        }
        else
        {
            /* Add a new label node to the symbol list */
            next_node(*&list, pLINE->label, IC + 100, SYMBOL_CODE);
            IC++;
        }
        break;

    case (PARSED_LABEL | PARSED_MACRO):
        ERROR_MSG("No label is allowed on a macro line");
        ERR_FLAG = TRUE;
        break;

    case (PARSED_LABEL | PARSED_DIRECTIVE):
        /* Check for existing label with the same name (excluding SYMBOL_ENTRY) */
        if ((search_list(*list, pLINE->label, &value, &property) != ERROR) && property != SYMBOL_ENTRY)
        {
            ERROR_MSG("Label name was already set");
            ERR_FLAG = TRUE;
            break;
        }
        else
        {
            /* Handle directives like .string or .data with a label */
            if (pLINE->parsed->type == SYMBOL_DATA_STRING)
            {
                /* Add symbol for a string directive */
                next_node(*&list, pLINE->label, DC, SYMBOL_DATA_STRING);
                for (i = 0; i < pLINE->len; i++)
                {
                    data_arr[DC + i].reg = pLINE->parsed->symbol->directive->data[i];
                }
            }
            else if (pLINE->parsed->type == SYMBOL_DATA_NUMBERS)
            {
                /* Add symbol for a data directive with numbers */
                next_node(*&list, pLINE->label, DC, SYMBOL_DATA_NUMBERS);
                for (i = 0; i < pLINE->len; i++)
                {
                    data_arr[DC + i].reg = pLINE->parsed->symbol->directive->data[i];
                }
            }
            else if (pLINE->parsed->type == SYMBOL_EXTERNAL)
            {
                /* Add symbol for an .EXTERNAL directive with a label */
                next_node(*&list, pLINE->label, IC + 100, SYMBOL_CODE);
                /* No need to update DC for external symbols */
                break;
            }
            DC += pLINE->len;
        }
        break;

    case (PARSED_LABEL | PARSED_INSTRUCTION):
        /* Check for existing label with the same name */
        if (search_list(*list, pLINE->label, NULL, NULL) != ERROR)
        {
            strcpy(tmp, pLINE->label); /* Copy label name to temporary variable */
            next_node(*&list, tmp, IC + 100, SYMBOL_CODE);
        }
        else
        {
            ERROR_MSG("Label name was already set");
            ERR_FLAG = TRUE;
        }
        encode_inst(pLINE, list);
        break;

    default:
        break;
    }

    return ERR_FLAG;
}

int conv_addmod(int mode)
{
    /* Function to convert addressing mode constants to internal representation
     *
     * This function takes an addressing mode constant (`mode`) as input.
     * It then uses a switch statement to map the input mode to an internal representation:
     *   - ADDMODE_0: Returns 0 (likely an immediate addressing mode)
     *   - ADDMODE_1: Returns 1 (likely a direct addressing mode)
     *   - ADDMODE_2: Returns 2 (likely an indirect addressing mode)
     *   - ADDMODE_3: Returns 3 (likely a register addressing mode)
     *
     * If the input mode doesn't match any of the defined cases, the function returns ERROR.
     */
    switch (mode)
    {
    case ADDMODE_0:
        return 0;
    case ADDMODE_1:
        return 1;
    case ADDMODE_2:
        return 2;
    case ADDMODE_3:
        return 3;
    }
    return ERROR;
}

int entry_encode(char *entry, line_t *pLINE, symbol_node **list)
{
    /* Function to handle the .ENTRY directive
     *
     * This function takes the entry point name (`entry`), parsed line information (`pLINE`),
     * and symbol list (`list`) as arguments. It adds a new symbol node to the list for the entry point
     * with the following properties:
     *   - Name: The entry point name (`entry`)
     *   - Value: IC + 100 (likely reserving space for the entry point instruction)
     *   - Property: SYMBOL_ENTRY (indicating an entry point symbol)
     *
     * The function always returns TRUE, signifying successful handling of the .ENTRY directive.
     */
    next_node(*&list, entry, IC + 100, SYMBOL_ENTRY);
    return TRUE;
}

void build_absolute_word(symbol_node *list, char *name, int opvalue, int bSRC, int index)
{
    /* Function to build an absolute instruction word
     *
     * This function takes various arguments to handle building an absolute instruction word:
     *   - list: Symbol table list
     *   - name: Operand name (can be a register, symbol name, or numeric literal)
     *   - opvalue: Opcode value (if applicable)
     *   - bSRC: Flag indicating whether the operand is source (TRUE) or destination (FALSE)
     *   - index: Index of the instruction word in the instruction array
     *
     * The function performs the following steps to build the absolute instruction word:
     *   1. If `opvalue` is valid and `name` is empty, it directly stores the opcode value
     *      shifted left by 2 bits and sets the addressing mode to absolute (ABSOLUTE).
     *   2. If `name` represents a register and `bSRC` is TRUE (source operand), it retrieves the
     *      register address using `reg_address` and sets the addressing mode to absolute. Similarly,
     *      it handles the case where `bSRC` is FALSE (destination operand).
     *   3. If a symbol with the name `name` exists in the symbol table (`search_list`), it retrieves
     *      the symbol's value and stores it shifted left by 2 bits in the instruction word.
     *   4. If `name` is a valid numeric literal (within the range of representable values, checked using
     *      `is_num`), it stores the numeric value shifted left by 2 bits in the instruction word.
     *   5. If none of the above conditions are met, it prints an error message and returns.
     */
    int value;
    if (is_valid(opvalue) && !name)
    {
        instruction_arr[index].reg = (opvalue << 2) | ABSOLUTE;
        return;
    }
    if (is_register(name) != ERROR)
    {
        if (bSRC)
        {
            /* SRC IS REGISTER */
            instruction_arr[index].reg = reg_address(name, TRUE) | ABSOLUTE;
            return;
        }
        /* DST IS REGISTER */
        instruction_arr[index].reg = reg_address(name, FALSE) | ABSOLUTE;
        return;
    }
    if ((search_list(list, name, &value, NULL) != ERROR))
    {
        /* DST IS REGISTER */
        instruction_arr[index].reg = reg_address(name, FALSE) | ABSOLUTE;
        return;
    }
    if ((search_list(list, name, &value, NULL) != ERROR))
    {
        instruction_arr[index].reg = value << 2;
        return;
    }
    if ((value = is_num(name) != _12BIT_MIN))
    {
        instruction_arr[index].reg = value << 2;
        return;
    }
    ERROR_MSG("Failed to create absolute word");
    return;
}

void build_relocatable_word(symbol_node *list, char *name, int index)
{
    /* Function to build a relocatable instruction word
     *
     * This function takes the symbol table list (`list`), operand name (`name`), and instruction word
     * index (`index`) as arguments. It performs the following steps to build a relocatable instruction word:
     *   1. It attempts to find the symbol with the name `name` in the symbol table using `search_list`.
     *      It retrieves the symbol's value and property if found.
     *   2. If the symbol is found and its property excludes SYMBOL_EXTERNAL and SYMBOL_ENTRY (using bitwise
     *      AND and negation), it stores the symbol's value shifted left by 2 bits and sets the addressing mode
     *      to relocatable (RELOCATABLE). This indicates that the operand refers to a symbol that might be
     *      defined later during assembly.
     *   3. If the symbol isn't found or has the SYMBOL_EXTERNAL or SYMBOL_ENTRY property, it creates a default
     *      external reference by setting the instruction word register field to 0 and the addressing mode to
     *      external (EXTERNAL). This signifies that the operand refers to a symbol defined in a separate assembly
     *      module.
     */
    int value = -1, property = -1;
    search_list(list, name, &value, &property);
    if ((value != ERROR) && (property & ~(SYMBOL_EXTERNAL | SYMBOL_ENTRY)))
    {
        instruction_arr[index].reg = (value << 2) | RELOCATABLE;
        return;
    }
    else
    {
        instruction_arr[index].reg = 0 | EXTERNAL; /* if not found create a default external reference */
    }
    return;
}

void build_external_word(symbol_node *list, char *name, int index)
{
    /* Function to build an external instruction word
     *
     * This function takes the symbol table list (`list`), operand name (`name`), and instruction word
     * index (`index`) as arguments. It performs the following steps to build an external instruction word:
     *   1. It attempts to find the symbol with the name `name` in the symbol table using `search_list_property`,
     *      specifying the SYMBOL_EXTERNAL property to search for.
     *   2. If the symbol is found with the SYMBOL_EXTERNAL property, it sets the instruction word register
     *      field to 0 and the addressing mode to external (EXTERNAL).
     *   3. If the symbol isn't found with the SYMBOL_EXTERNAL property, it prints an error message and returns.
     */
    int value;
    if (search_list_property(list, name, &value, SYMBOL_EXTERNAL))
    {
        instruction_arr[index].reg = 0 | EXTERNAL;
        return;
    }
    ERROR_MSG("Failed to find external symbol");
    return;
}

int reg_address(char *reg, int src)
{
    /* Function to determine register address based on register name and source/destination flag
     *
     * This function takes the register name (`reg`) and a flag indicating whether it's a source operand (`src`)
     * as arguments. It performs the following steps to determine the register address:
     *   1. It attempts to find the register number using `is_register`. If the register name is invalid, it returns
     *      an error code (ERROR).
     *   2. If `src` is TRUE (source operand), it shifts the register number left by 5 bits. This likely positions
     *      the register number in the appropriate bit positions for source operands in the instruction word.
     *   3. If `src` is FALSE (destination operand), it shifts the register number left by 2 bits. This likely positions
     *      the register number in the appropriate bit positions for destination operands in the instruction word.
     *   4. The function returns the calculated register address.
     */
    int regnum = ERROR;
    if ((regnum = is_register(reg)) == ERROR)
        return ERROR;
    if (src)
        return (regnum << 5);
    return (regnum << 2);
}

void encode_inst(line_t *pLINE, symbol_node **list)
{
    int i = 1, src_addmode, dst_addmode, src_are, dst_are, src_index = -1, dst_index = -1;
    int src_val, dst_val;

    char *src_name, *dst_name;
    int property, value, opcode, reg;
    property = value = opcode = reg = 0;
    src_name = (char *)calloc(sizeof(char), MACRO_LEN);
    dst_name = (char *)calloc(sizeof(char), MACRO_LEN);

    /* Addressing mode */
    src_addmode = pLINE->parsed->symbol->instruction->source->addmod;
    dst_addmode = pLINE->parsed->symbol->instruction->destination->addmod;

    /* label name or register */
    if (!is_valid(pLINE->parsed->symbol->instruction->source->op->value))
        strcpy(src_name, pLINE->parsed->symbol->instruction->source->op->name);
    if (!is_valid(pLINE->parsed->symbol->instruction->destination->op->value))
        strcpy(dst_name, pLINE->parsed->symbol->instruction->destination->op->name);

    /* numeric values */
    src_val = pLINE->parsed->symbol->instruction->source->op->value;
    dst_val = pLINE->parsed->symbol->instruction->destination->op->value;

    /* index values */
    if (!is_name(pLINE->parsed->symbol->instruction->source->str_index))
        src_index = pLINE->parsed->symbol->instruction->source->index;
    else
        src_index = is_num(pLINE->parsed->symbol->instruction->source->str_index);
    if (!is_name(pLINE->parsed->symbol->instruction->destination->str_index))
        dst_index = pLINE->parsed->symbol->instruction->destination->index;
    else
        dst_index = is_num(pLINE->parsed->symbol->instruction->destination->str_index);

    /* are property */
    src_are = pLINE->parsed->symbol->instruction->source->are;
    dst_are = pLINE->parsed->symbol->instruction->destination->are;

    opcode = pLINE->parsed->symbol->instruction->opcode << 6;
    reg = opcode | (conv_addmod(src_addmode) << 4) | (conv_addmod(dst_addmode) << 2);

    /* INSTRUCTION FIRST WORD */
    instruction_arr[IC].reg = reg;

    instruction_arr[IC].src_name = src_name;
    instruction_arr[IC].dst_name = dst_name;

    /* The source and the destination are registers */
    if ((is_register(src_name) != ERROR) && (is_register(dst_name) != ERROR))
    {
        instruction_arr[IC + 1].reg = reg_address(src_name, TRUE) | reg_address(dst_name, FALSE);
        IC += 2;
        SAFE_FREE(src_name)
        SAFE_FREE(dst_name)
        return;
    }

    /* SRC WORD */
    if (pLINE->len > 2)
    {
        instruction_arr[IC].src_addmod = src_addmode;

        search_list_property(*list, src_name, &value, SYMBOL_EXTERNAL);

        if (value)
        {
            build_external_word(*list, src_name, IC + i);
            instruction_arr[IC].len = pLINE->len;
            strcpy(instruction_arr[IC].name, src_name);
            instruction_arr[IC].type = SYMBOL_EXTERNAL;
        }

        else if (src_are == ABSOLUTE)
        {
            if (!*src_name)
                build_absolute_word(*list, NULL, src_val, TRUE, IC + i);
            else
                build_absolute_word(*list, src_name, 0, TRUE, IC + i);
        }
        else if ((search_list(*list, src_name, &value, &property) != ERROR) && (property & SYMBOL_MACRO))
            build_relocatable_word(*list, src_name, IC + i);
        i++;

        if (src_addmode == ADDMODE_2)
        {
            if (src_index != _12BIT_MIN)
                build_absolute_word(*list, NULL, src_index, TRUE, IC + i);
            else
                build_relocatable_word(*list, pLINE->parsed->symbol->instruction->source->str_index, IC + i);

            i++;
        }
        /* everything else will get built on the second pass */
    }
    /* DST WORD */
    if (pLINE->len > 1)
    {
        instruction_arr[IC].dst_addmod = dst_addmode;
        search_list_property(*list, dst_name, &value, SYMBOL_EXTERNAL);

        if (value)
        {
            build_external_word(*list, dst_name, IC + i);
            instruction_arr[IC].len = pLINE->len;
            strcpy(instruction_arr[IC].name, dst_name);
            instruction_arr[IC].type = SYMBOL_EXTERNAL;
        }

        if (dst_are == ABSOLUTE)
        {
            if (!*dst_name)
                build_absolute_word(*list, NULL, dst_val, FALSE, IC + i);
            else
                build_absolute_word(*list, dst_name, 0, FALSE, IC + i);
        }
        /* only macros */
        else if ((search_list(*list, src_name, &value, &property) != ERROR) && (property & SYMBOL_MACRO))
            build_relocatable_word(*list, dst_name, IC + i);

        i++;

        if (dst_addmode == ADDMODE_2)
        {
            if (dst_index != _12BIT_MIN)
                build_absolute_word(*list, NULL, dst_index, TRUE, IC + i);
            else if ((search_list(*list, pLINE->parsed->symbol->instruction->destination->str_index, &value, &property) != ERROR) && (property & SYMBOL_MACRO))
                build_absolute_word(*list, pLINE->parsed->symbol->instruction->destination->str_index, _12BIT_MIN, FALSE, IC + i);
            else
                build_relocatable_word(*list, pLINE->parsed->symbol->instruction->destination->str_index, IC + i);

            i++;
        }
        /* everything else will get built on the second pass */
    }
    instruction_arr[IC].len = pLINE->len;
    IC += pLINE->len;
    /* free_symbol(pLINE->parsed); */
    return;
}

void complete_encoding(symbol_node *list, int oIC)
{
    int i = 0, src_addmode, dst_addmode;
    int property_src, property_dst;
    char *src, *dst;

    do
    {
        src = instruction_arr[i].src_name;
        dst = instruction_arr[i].dst_name;
        src_addmode = instruction_arr[i].src_addmod;
        dst_addmode = instruction_arr[i].dst_addmod;

        trim_white(src);
        trim_white(dst);
        if (src)
            search_list(list, src, NULL, &property_src);
        if (dst)
            search_list(list, dst, NULL, &property_dst);

        /* Two words of addmode 2 */
        if (instruction_arr[i].len == 5)
        {
            build_relocatable_word(list, src, i + 1);
            build_relocatable_word(list, dst, i + 3);
            i += instruction_arr[i].len;
        }
        /* Each operand is of type 1 or 2 */
        else if (src_addmode & ADDMODE_12 && dst_addmode & ADDMODE_12)
        {
            if (src_addmode & ADDMODE_2)
            {
                build_relocatable_word(list, src, i + 1);
                build_relocatable_word(list, dst, i + 3);
            }
            else
            {
                build_relocatable_word(list, src, i + 1);
                build_relocatable_word(list, dst, i + 2);
            }
            i += instruction_arr[i].len;
        }
        else if (src_addmode & ADDMODE_12)
        {
            build_relocatable_word(list, src, i + 1);
            i += instruction_arr[i].len;
        }
        else if (dst_addmode & ADDMODE_12)
        {
            if (src_addmode & ADDMODE_ALL)
                build_relocatable_word(list, dst, i + 2);
            else
                build_relocatable_word(list, dst, i + 1);
            i += instruction_arr[i].len;
        }
        else
            i++;

    } while (i < oIC);

    return;
}

void get_entries(symbol_node *list)
{
    int i, j = i = 0;
    symbol_node *tmp = list;
    while (tmp)
    {
        if (tmp->property == SYMBOL_ENTRY)
            strcpy(entries[i++].name, tmp->name);
        tmp = tmp->next;
    }
    tmp = list;
    while (tmp)
    {
        j = 0;
        do
        {
            if (tmp->property & (SYMBOL_CODE | SYMBOL_DATA_NUMBERS | SYMBOL_DATA_STRING) && strcmp_hash(tmp->name, entries[j].name))
                entries[j].reg = tmp->value;
        } while (j++ < i);
        tmp = tmp->next;
    }
    tmp = list;
}
