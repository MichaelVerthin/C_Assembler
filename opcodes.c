#include <string.h>
#include <ctype.h>
#include "opcodes.h"
#include "globals.h"

int num_operands(char *line);

const int
opcodes[OPCODE_NUM][4] = 
{
/*  {OPCODE,  SOURCE , DESTINATION, OPERANDS_NUM} */
    {MOV, ADDMODE_ALL, ADDMODE_123, 2},     /* index 0 */
    {CMP, ADDMODE_ALL, ADDMODE_ALL, 2},     /* index 1 */
    {ADD, ADDMODE_ALL, ADDMODE_123, 2},     /* index 2 */
    {SUB, ADDMODE_ALL, ADDMODE_123, 2},     /* index 3 */
    {NOT, ADDMODE_NONE, ADDMODE_123, 1},    /* index 4 */
    {CLR, ADDMODE_NONE, ADDMODE_123, 1},    /* index 5 */
    {LEA, ADDMODE_12, ADDMODE_123, 2},      /* index 6 */
    {INC, ADDMODE_NONE, ADDMODE_123, 1},    /* index 7 */
    {DEC, ADDMODE_NONE, ADDMODE_123, 1},    /* index 8 */
    {JMP, ADDMODE_NONE, ADDMODE_13, 1},     /* index 9 */
    {BNE, ADDMODE_NONE, ADDMODE_13, 1},     /* index 10 */
    {RED, ADDMODE_NONE, ADDMODE_123, 1},    /* index 11 */
    {PRN, ADDMODE_NONE, ADDMODE_ALL, 1},    /* index 12 */
    {JSR, ADDMODE_NONE, ADDMODE_13, 1},     /* index 13 */
    {RTS, ADDMODE_NONE, ADDMODE_NONE, 0},   /* index 14 */
    {STOP, ADDMODE_NONE, ADDMODE_NONE, 0}   /* index 15 */
};


int
check_operands(char *line, unsigned ins)
{
    int ops = num_operands(line);
    int c_ops = opcodes[ins][OPERANDS_NUM];
    if(c_ops == ops)
    {
        return ops;
    }    
    else if (c_ops > ops)
    {
        ERROR_NUM("Not enough operands", c_ops)
        return ERROR;
    }
    else
    {
        ERROR_NUM("Too many operands", c_ops)
        return ERROR;
    }    
}

/**
 * Count all commas in line after label.
 * number of operands equal to the number of commas plus 1.
 */
int
num_operands(char *line)
{
    int ops = 1;
    const char *c = line;
    if(strcmp_hash(line, "stop") || strcmp_hash(line, "rts"))
        return 0;
    while(*c)
    {
        if(*c++ == ',')
            ops++;
    }
    return ops;
}

/**
 * parse the operand for the correct addressing mode used
 */
int
get_addmode(char *operand, unsigned code, int mode, int *abs, char *p_macro)
{
    /* Supported addressing mode for current opcode */
    int sup_mode = opcodes[code][mode];
    int tmplen;
    char tmp[LINE_LEN];
    char *macro;

    strcpy(tmp, operand);
    tmplen = strlen(tmp);
    if((sup_mode & ADDMODE_0) && *(tmp) == '#' && (is_name(++operand) || is_num(operand)))
    {
        if(is_num(tmp))
            *abs = TRUE;
        else
            *abs = FALSE;
        return ADDMODE_0;
    }
    else if((sup_mode & ADDMODE_3) && (is_register(operand) != ERROR))
    {
        *abs = TRUE;
        return ADDMODE_3;
    }
    else if ((sup_mode & ADDMODE_2) && is_name(strtok(tmp, "[")) && (tmplen != strlen(tmp)))
    {
            strcpy(p_macro, tmp);
            macro = strtok(NULL, "]");
            if(is_name(macro))
            {
                *abs = FALSE;
                return ADDMODE_2;
            }
            else if (is_num(macro))
            {
                *abs = TRUE;
                return ADDMODE_2;
            }
            else
            {
                ERROR_MSG("Failed to interpret the appropriate addressing mode")
                return ERROR;
            }        
    }
    else if((sup_mode & ADDMODE_1) && is_name(operand))
    {
        return ADDMODE_1;
    }
    
    else
    {
        ERROR_MSG("Failed to interpret the appropriate addressing mode\n\tMost likely unsupported addressing mode for the requested opcode")
        return ERROR;
    }
    return ERROR;
}

int
addmod_sz(int mode)
{
    if(mode & ADDMODE_2)
        return 2;
    return 1;
}

int
get_are(int mode)
{
    switch (mode)
    {
    case ADDMODE_0:
        return ABSOLUTE;
    case ADDMODE_3:
        return ABSOLUTE;
    case ADDMODE_1:
        return RELOCATABLE | EXTERNAL;
    case ADDMODE_2:
        return RELOCATABLE | EXTERNAL;
    default:
        break;
    }
    return ERROR;
}