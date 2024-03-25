#ifndef MISC_H
#define MISC_H

#include "line.h"

#define SAFE_FREE(p) \
    {                \
        free(p);     \
        (p) = NULL;  \
    }
#define ZERO_MEMORY(dst, len) memset((dst), 0, (len))
#define IS_ASCII(c) (((c) & ~0x7f) == 0) /* If C is a 7 bit value.  */
#define INIT_PROTECTED_SIZE 10

extern unsigned long protected[INIT_PROTECTED_SIZE];

/* Math functions */
int is_valid(int);

void LINE_FREE(line_t *);
/* Hashing */
unsigned long hash(unsigned char *);
int strcmp_hash(char *, char *);
int is_reserved(char *);
int insert_protected(char *);

#endif