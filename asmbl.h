/* asmbl.h -- primary header for main */
#ifndef ASMBL_H
#include <stdlib.h>
#include <stdio.h>
#include "line.h"
#define ASMBL_H
#define _12BIT_MAX 0xFFF  /* Biggest number for integer in word_t */
#define _12BIT_MIN -0xFFF /* Smallest number for integer in word_t */
/*#define LINE_LEN 128      /* Line length */
#define MEM_SIZE 4096 /* Number of cells of memory, each 14 bits */
#define OPCODE_NUM 16
#define TXT_FILE "test.txt" /* File to read, assembly language */

/* Opcodes Instructions */
#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define NOT 4
#define CLR 5
#define LEA 6
#define INC 7
#define DEC 8
#define JMP 9
#define BNE 10
#define RED 11
#define PRN 12
#define JSR 13
#define RTS 14
#define HLT 15
/**********************/

#define IMMIDIATE 0
#define NEGATIVE '-'
#define POSITIVE '+'

enum BOOL
{
    FALSE,
    TRUE
};
enum ARE_E
{
    A = 0,
    R = 1,
    E = 2
};
/**
 * word datatype
 * each is of size of 14 bits.
 */
struct
{
    unsigned int are : 2;   /* A.R.E addressing method */
    unsigned int dst : 2;   /* destination operand */
    unsigned int src : 2;   /* source operand */
    unsigned int opcd : 4;  /* opcode */
    unsigned int unusd : 4; /* unused: initialized to 0 */
} word_t;
typedef struct word_t WORD_T;

struct
{
    char *name;
    unsigned int reg : 14;
} _register_t;
typedef struct _register_t REGISTER;

int IC; /* Instruction Counter */

#endif
