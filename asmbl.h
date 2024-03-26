/* asmbl.h -- primary header for main */

/*
 * Guard macro to prevent multiple inclusions of this header file.
 * This ensures that the header file's content is defined only once
 * during compilation, avoiding potential naming conflicts.
 */
#ifndef ASMBL_H
#define ASMBL_H

/*
 * Standard library inclusions
 * These headers provide essential functionalities for input/output (stdio.h)
 * and memory management (stdlib.h).
 */
#include <stdlib.h>
#include <stdio.h>

/*
 * Inclusion of line.h (likely containing line parsing related functions)
 * This header file is assumed to be specific to the line parsing aspects of the assembly process.
 */
#include "line.h"

/*
 * Constant definitions
 * These constants represent various limits and sizes used throughout the assembly process:
   - _12BIT_MAX: Maximum representable value for an integer in a word_t (0xFFF)
   - _12BIT_MIN: Minimum representable value for an integer in a word_t (-0xFFF)
 */
#define _12BIT_MAX 0xFFF
#define _12BIT_MIN (-0xFFF)

/*
 * Error code definitions
 * These constants represent various error codes used for signaling different types of errors during assembly:
   - NO_FILE (127): Error code for missing input file
   - EXCEEDS_MAX_LENGTH (2): Error code for exceeding maximum line length
   - PARSING_FAILURE (3): Error code for failure during parsing
 */
#define NO_FILE 127
#define EXCEEDS_MAX_LENGTH 2
#define PARSING_FAILURE 3

/*
 * Memory and instruction/register related constants
 * These constants define the size of memory (MEM_SIZE), number of opcodes (OPCODE_NUM), and number of registers (REGISTER_NUM) used in the assembly process.
 */
#define MEM_SIZE 4096  /* Number of memory cells (each 14 bits) */
#define OPCODE_NUM 16  /* Number of available opcodes */
#define REGISTER_NUM 8 /* Number of available registers */

/*
 * Initial allocation sizes for instruction and data arrays
 * These constants define the initial allocation sizes for the instruction array (INITIAL_IC_ARR) and data array (INITIAL_DC_ARR) used during assembly.
 */
#define INITIAL_IC_ARR 100
#define INITIAL_DC_ARR 100

/*
 * Line, label, and macro length limitations
 * These constants define the maximum lengths for lines (LINE_LEN), labels (LABEL_LEN), and macros (MACRO_LEN) used in the assembly process.
 */
#define LINE_LEN 80  /* Maximum length of a line in the assembly code */
#define LABEL_LEN 31 /* Maximum length of a label name */
#define MACRO_LEN 20 /* Maximum length of a macro name (including ".define" or ".data") */

/*
 * Opcode instruction definitions
 * These constants represent the opcode values for various assembly instructions:
   - MOV: Move instruction
   - CMP: Compare instruction
   - ADD: Addition instruction
   - ... (other instructions)
   - HLT: Halt instruction
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
#define HLT 15

/*
 * Special character constants for positive and negative signs
 * These constants represent the characters used to denote positive and negative values.
 */
#define NEGATIVE '-'
#define POSITIVE '+'

/*
 * Boolean data type definition
 * This defines an enumeration (enum) for representing Boolean values (TRUE and FALSE).
 */
enum BOOL
{
    FALSE,
    TRUE
};

/*
 * WORD_T data type definition (continued)
 * - src_name: name of the source operand (label or macro)
 * - src_index: index register used in source addressing (if applicable)
 * - dst_name: name of the destination operand (label or macro)
 * - dst_index: index register used in destination addressing (if applicable)
 * - dst_addmod: addressing mode for the destination operand
 * - name: actual name used for the word (for searching or output)
 * - reg: 14-bit signed integer value stored in the word
 */
struct WORD_T
{
    int type;
    int len;              /* number of words */
    int src_addmod;       /* source word addressing mode */
    char *src_name;       /* if the source is a label or a macro */
    char *src_index;      /* if the source is of addmode 2 and needs an index */
    char *dst_name;       /* if the destination is a label or a macro */
    char *dst_index;      /* if the destination is of addmode 2 and needs an index */
    int dst_addmod;       /* destination word addressing mode */
    char name[MACRO_LEN]; /* Actual used name to be searched in the list or outputed to file */
    signed int reg : 14;  /* actual word */
};
typedef struct WORD_T WORD_T;

/*
 * Declarations of global arrays for instructions, data, and entries
 * These external declarations reference the instruction array (`instruction_arr`), data array (`data_arr`), and entries array (`entries`) defined elsewhere.
 * Their sizes are initially set using the constants `INITIAL_IC_ARR`, `INITIAL_DC_ARR`, and a fixed size of 20 for entries, respectively.
 */
extern WORD_T instruction_arr[INITIAL_IC_ARR];
extern WORD_T data_arr[INITIAL_DC_ARR];
extern WORD_T entries[20];

#endif
