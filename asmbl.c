#include <stdio.h>
#include <stdlib.h>
#include "asmbl.h"

static register_t r0, r1, r2, r3, r4, r5, r6, r7;

int main(int argc, char const *argv[])
{
    FILE *fptr;
    char *line; /* each line in input file */
    fptr = fopen(TXT_FILE, "r");
    if (!fptr)
    {
        fprintf(stderr, "Failed to open file %s\n", TXT_FILE);
        exit(EXIT_FAILURE);
    }
    line = (char *)malloc(sizeof(char) * LINE_LEN);
    while (fgets(line, LINE_LEN, fptr))
    {
        fprintf(stdout, "%s", line);
    }
    printf("the size of word_s is: %d bytes\n", sizeof(word_s));
    printf("the size of register_t is: %d bytes\n", sizeof(register_t));
    printf("the size of test is: %d bytes\n", sizeof(test));

    fclose(fptr);
    return 0;
}