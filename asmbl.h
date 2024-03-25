/* asmbl.h -- primary header for main */

#define LINE_LEN 128        /* Line length */
#define MEM_SIZE 4096       /* Number of cells of memory, each 14 bits */
#define TXT_FILE "test.txt" /* File to read, assembly language */

enum OPCODES_E
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
    stop
};
/**
 * register datatype
 * each is of size of 14 bits.
 */
struct
{
    unsigned int are : 2;   /* A.R.E addressing method */
    unsigned int dst : 2;   /* destination operand */
    unsigned int src : 2;   /* source operand */
    unsigned int opcd : 4;  /* opcode */
    unsigned int unusd : 4; /* unused: initialized to 0 */
} word_s;

struct test
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int e;
} test;

typedef struct
{
    char *name;
    unsigned int reg : 14;
} register_t;
