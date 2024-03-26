#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "asmbl.h"   /* Include header for assembly language related functions */
#include "pass.h"    /* Include header for functions related to assembly passes */
#include "misc.h"    /* Include header for miscellaneous utility functions */
#include "files.h"   /* Include header for functions related to file operations */
#include "globals.h" /* Include header for global variables used throughout assembly */

/*
 * The main function is the entry point of the assembly program.
 * It coordinates the assembly process for multiple input files.
 */
int main(int argc, char const *argv[])
{
    /* Variable to store the return value of pass functions (0 for success, non-zero for error) */
    int pass_return;

    /* File pointers for input, output (object, entry, external) files */
    FILE *fptr, *pout, *pext, *pent;

    /* Symbol table node to store symbol information and linked list pointer */
    symbol_node olist;
    symbol_node *list = &olist;

    /* Initialize symbol table node (empty symbol initially) */
    olist.name = NULL;
    olist.next = NULL;
    olist.property = 0;
    olist.value = 0;

    /* Check for missing input files */
    if (argc < 2)
    {
        fprintf(stderr, "No input files provided.\nAssembling terminated.\n");
        return NO_FILE;
    }

    /* Loop through each input file provided as command line arguments */
    while (--argc)
    {
        int ic;                            /* Instruction counter for assembly process */
        const char *filename = argv[argc]; /* Current file name being processed */

        /* Copy filename for output file manipulation (avoid modifying original argument) */
        char cpyfile[40];
        strcpy(cpyfile, filename);

        /* Validate filename extension to ensure it's a valid assembly file (.as) */
        if (!validate_filename(filename))
        {
            fprintf(stderr, "The assembler only accepts files with the .as extension.\n");
            continue;
        }

        fprintf(stdout, "Assembling file %s...\n", filename);

        /* Open the input file for reading */
        fptr = fopen(filename, "r");
        if (!fptr)
        {
            fprintf(stderr, "Failed to open file %s, error %d\n", filename, errno);
            continue;
        }

        /* Perform the first pass of the assembly process (parsing and symbol table generation) */
        pass_return = first_pass(fptr, &list);
        if (pass_return)
        {
            fprintf(stderr, "First pass failed for file %s! Terminating...\n", filename);
            fclose(fptr);
            continue;
        }

        /* Store instruction counter value from the first pass */
        ic = IC;

        /* Perform the second pass of the assembly process (machine code generation) */
        pass_return = second_pass(fptr, &list, ic);
        IC = ic; /* Update global instruction counter after second pass */
        if (pass_return)
        {
            fprintf(stderr, "Second pass failed for file %s! Terminating...\n", filename);
            fclose(fptr);
            continue;
        }

        /*
         *  Output file creation and formatting section
         *  This section creates three output files: object (.ob), entry point (.ent), and external references (.ext).
         */
        pout = create_ext(cpyfile, ".ob");  /* Create object file */
        pent = create_ext(cpyfile, ".ent"); /* Create entry point file */
        pext = create_ext(cpyfile, ".ext"); /* Create external references file */

        /* Extract filename without extension using strtok (modifies cpyfile) */
        strtok(cpyfile, ".");

        /* Check if all output files were created successfully */
        if (pout && pent && pext)
        {
            fout(pout);                       /* Write object file content */
            fout_entext(cpyfile, pext, pent); /* Write entry point and external references to respective files */
        }
        else
        {
            fprintf(stderr, "Failed to create output files - object, entry or external\n");
            return EXIT_FAILURE;
        }
        /* Close the input file */
        fclose(fptr);
        /* Print message indicating successful assembly completion for the current file */
        fprintf(stdout, "Fishished assembling file %s\n", filename);
    }
    return EXIT_SUCCESS;
}
