/* asmbl.h -- primary header for main */
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "pass.c"
#define LINE_LEN 128  /* Line length */
#define MEM_SIZE 4096 /* Number of cells of memory, each 14 bits */
#define OPCODE_NUM 16
#define TXT_FILE "test.txt" /* File to read, assembly language */

enum BOOL
{
    FALSE = 0,
    TRUE = 1
};
/* enum OPCODES_E
{
    mov,
    cmp,
    add,
    sub,
    not,
    clr,
    lea,
    inc,
    dec,
    jmp,
    bne,
    red,
    prn,
    jsr,
    rts,
    hlt
}; */
enum ARE_E
{
    A = 0,
    R = 1,
    E = 10
};
/**
 * word datatype
 * each is of size of 14 bits.
 */
struct word_s
{
    unsigned int are : 2;   /* A.R.E addressing method */
    unsigned int dst : 2;   /* destination operand */
    unsigned int src : 2;   /* source operand */
    unsigned int opcd : 4;  /* opcode */
    unsigned int unusd : 4; /* unused: initialized to 0 */
} word_t;
struct register_s
{
    char *name;
    unsigned int reg : 14;
} register_t;
typedef struct word_t WORD_T;
typedef struct register_t REGISTER;
