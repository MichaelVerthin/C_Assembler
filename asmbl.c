#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "asmbl.h"

static register_t r0, r1, r2, r3, r4, r5, r6, r7;

int main(int argc, char const *argv[])
{
    FILE *fptr;
    char *line; /* each line in input file */
    if (argc < 2)
    {
        fprintf(stderr, "No input files\nAssembling terminated\n");
        return EXIT_FAILURE;
    }
    fptr = fopen(argv[1], "r");
    if (!fptr)
    {
        fprintf(stderr, "Failed to open file %s, %d\n", argv[1], errno);
        return EXIT_FAILURE;
    }
    /* Reading line by line, each line is dynamically allocated */
    line = (char *)malloc(sizeof(char) * LINE_LEN);
    while (fgets(line, LINE_LEN, fptr))
    {
        fprintf(stdout, "%s", line);
    }

    fclose(fptr);
    return 0;
}