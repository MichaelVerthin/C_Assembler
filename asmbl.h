/* asmbl.h -- primary header for main */
#include <stdlib.h>
#include <stdio.h>
#define _13BIT_MAX 0x1FFF /* Biggest number for integer in word_t */
#define _13BIT_MIN 0xE000 /* Smallest number for integer in word_t */
#define LINE_LEN 128      /* Line length */
#define MEM_SIZE 4096     /* Number of cells of memory, each 14 bits */
#define OPCODE_NUM 16
#define TXT_FILE "test.txt" /* File to read, assembly language */
enum BOOL
{
    FALSE,
    TRUE
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
struct
{
    unsigned int are : 2;   /* A.R.E addressing method */
    unsigned int dst : 2;   /* destination operand */
    unsigned int src : 2;   /* source operand */
    unsigned int opcd : 4;  /* opcode */
    unsigned int unusd : 4; /* unused: initialized to 0 */
} word_t;
struct
{
    char *name;
    unsigned int reg : 14;
} _register_t;
typedef struct word_t WORD_T;
typedef struct _register_t REGISTER;
// int first_pass(FILE *fptr)
// {
//     char *line = (char *)malloc(sizeof(char) * LINE_LEN);

//     /* Reading line by line of the file*/
//     while (fgets(line, LINE_LEN, fptr) && line[0] != ';')
//     {
//         /* FIRST PASS */

//         /* Is MACRO? */
//         /**
//          * TODO - Configure Macro checker
//          */
//         /* Is Data Holder? such as array or variable? */
//         /**
//          * TODO - Configure array parser
//          */
//         /* .extern or .entry ? */
//         /**
//          * TODO - Configure .extern/.entry
//          */

//         fprintf(stdout, "%s", line);
//     }
//     return EXIT_SUCCESS;
// }

// int second_pass(FILE *fptr)
// {
//     return EXIT_SUCCESS;
// }