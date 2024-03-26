#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "line.h"
#include "misc.h"
#include "asmbl.h"

/* Free the line object and its dynamically allocated elements */
void LINE_FREE(line_t *oLINE)
{
    if (oLINE->label)
        SAFE_FREE(oLINE->label)
    if (oLINE->parsed)
        free_symbol(oLINE->parsed);
    /* if(oLINE->line)
        SAFE_FREE(oLINE->line)
    */
    SAFE_FREE(oLINE)
}

/*
 * Check if str is a legal number. Accepts '+' and '-' signs
 * returns _12BIT_MIN on illegal string.
 */
int is_num(char *str)
{
    char *ch;
    int num;
    if (!str)
        return _12BIT_MIN;
    num = (int)strtol(str, &ch, DECIMAL_BASE);

    if ((*ch == '\0') && is_valid(num))
        return num;
    return _12BIT_MIN;
}

/*
 * Check if string is legal.
 * Contains only ASCII characters without the space character.
 */
int is_string(const char *const s)
{
    const char *c;
    for (c = s; *c; c++)
        if (!IS_ASCII(*c))
            return 0;
    return 1;
}

int is_name(char *s)
{
    if (!s)
        return FALSE;
    return (is_string(s) && isalpha(s[0]) && !is_reserved(s));
}

/* Checks if str string is reserved and can be used */
int is_reserved(char *s)
{
    return (is_opcode(s) != -1) || (is_register(s) != -1);
}

int is_register(char *s)
{
    size_t i;
    char *reserved_registers[REGISTER_NUM] =
        {
            "r0", "r1", "r2", "r3",
            "r4", "r5", "r6", "r7"};

    for (i = 0; i < REGISTER_NUM; i++)
        if (strcmp_hash(s, reserved_registers[i]))
            return i;
    return -1;
}

int is_opcode(char *s)
{
    size_t i;
    char *reserved_opcodes[OPCODE_NUM] =
        {
            "mov", "cmp", "add", "sub",
            "not", "clr", "lea", "inc",
            "dec", "jmp", "bne", "red",
            "prn", "jsr", "rts", "hlt"};

    for (i = 0; i < OPCODE_NUM; i++)
        if (strcmp_hash(s, reserved_opcodes[i]))
            return i;
    return ILLEGAL_INST;
}

/* Trim whitespaces from end of string */
char *
trim_white(char *str)
{
    char *pos;
    int len;
    if (!str)
        return NULL;
    len = strlen(str);
    pos = str + len - 1;
    while (pos >= str && isspace(*pos))
    {
        *pos = '\0';
        pos--;
    }
    return str;
}

/* Trim whitespaces from start and end of string */
char *
clear_str(char *str)
{
    char *ch = str;
    while (ch && isspace(*ch++))
        ;
    if (ch)
        ch = trim_white(--ch);
    return ch;
}
