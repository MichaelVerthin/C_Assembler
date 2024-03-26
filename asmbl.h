/* asmbl.h -- primary header for main */
#ifndef ASMBL_H
#define ASMBL_H

#include <stdlib.h>
#include <stdio.h>
#include "line.h"

#define _12BIT_MAX 0xFFF  /* Biggest number for integer in word_t */
#define _12BIT_MIN (-0xFFF)  /* Smallest number for integer in word_t */
/********************************************************************/
/* Error Codes */
#define NO_FILE 127
#define EXCEEDS_MAX_LENGTH 2
#define PARSING_FAILURE 3

#define MEM_SIZE 4096      /* Number of cells of memory, each 14 bits */
#define OPCODE_NUM 16
#define REGISTER_NUM 8

#define INITIAL_IC_ARR 100
#define INITIAL_DC_ARR 100

#define LINE_LEN 80       /* Line maximum length */
#define LABEL_LEN 31      /* Label maximum length */
#define MACRO_LEN 20      /* macro maximum length - length of ".define" or ".data" */


/**
 * Opcodes Instructions
 */
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
#define STOP 15
/**********************/


#define NEGATIVE '-'
#define POSITIVE '+'
enum BOOL {FALSE, TRUE};



/**
 * word datatype
 * each is of size of 14 bits.
 */
struct WORD_T
{
    int type;
    int len;            /* number of words */
    int src_addmod;     /* source word addressing mode */
    char *src_name;     /* if the source is a label or a macro */
    char *src_index;    /* if the source is of addmode 2 and needs an index */
    char *dst_name;     /* if the destination is a label or a macro */
    char *dst_index;    /* if the destination is of addmode 2 and needs an index */
    int dst_addmod;     /* destination word addressing mode */
    char name[MACRO_LEN];         /* Actual used name to be searched in the list or outputed to file */
    signed int reg:14;  /* actual word */
};
typedef struct WORD_T WORD_T;

/* Instructions and Data binary words */
extern WORD_T instruction_arr[INITIAL_IC_ARR];
extern WORD_T data_arr[INITIAL_DC_ARR];
extern WORD_T entries[20];

#endif
