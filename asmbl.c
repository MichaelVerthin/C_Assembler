#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "asmbl.h"

/*
static REGISTER r0, r1, r2, r3, r4, r5, r6, r7;
static int IC = 0;
static int DC = 0;
*/

int main(int argc, char const *argv[])
{
    int pass_return;
    FILE *fptr; /* pointer to file object */
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
    pass_return = first_pass(fptr);
    if (pass_return)
    {
        perror("First pass Failed!\nTerminating\n");
        fclose(fptr);
        return EXIT_FAILURE;
    }
    pass_return = second_pass(fptr);
    if (pass_return)
    {
        perror("Second pass Failed!\nTerminating\n");
        fclose(fptr);
        return EXIT_FAILURE;
    }
    fclose(fptr);
    return 0;
}