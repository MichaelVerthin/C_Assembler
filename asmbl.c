#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "asmbl.h"
#include "pass.h"
#include "misc.h"
#include "files.h"
#include "globals.h"

/* 
static REGISTER r0, r1, r2, r3, r4, r5, r6, r7;
*/

/*
 * ─── START HERE ─────────────────────────────────────────────────────────────────
 */
int
main(int argc, char const *argv[])
{
    int pass_return;
    FILE *fptr, *pout, *pext, *pent;     /* pointer to file object */
    symbol_node olist;
    symbol_node *list = &olist;
    olist.name = NULL;
    olist.next = NULL;
    olist.property = 0;
    olist.value = 0;


    if (argc < 2)
    {
        fprintf(stderr, "No input files\nAssembling terminated.\n");
        return NO_FILE;
    }


    while(--argc)
    {
        int ic;
        const char *filename = argv[argc];
        char cpyfile[40];
        strcpy(cpyfile, filename);


        if(!validate_filename(filename))
        {
            fprintf(stderr, "The assembler accepts only .as files\n");
            continue;
        }

        fprintf(stdout, "Assembling file %s...\n", filename);

        fptr = fopen(filename, "r");
        if(!fptr)
        {
            fprintf(stderr, "Failed to open file %s, %d\n", filename, errno);
            continue;
        }
        pass_return = first_pass(fptr, &list);
        if (pass_return)
        {
            fprintf(stderr, "First pass failed! (%s)\nTerminating... %d\n",filename, 1);
            fclose(fptr);
            continue;
        }
        ic = IC;
        pass_return = second_pass(fptr, &list, ic);
        IC = ic;
        if (pass_return)
        {
            fprintf(stderr, "Second pass failed! (%s)\nTerminating... %d\n",filename, 1);
            fclose(fptr);
            continue;
        }


        /**
         * Creating and formatting output files
         * Creates 3 output files - object ,entry and external files.
         */
        pout = create_ext(cpyfile, ".ob");
        pent = create_ext(cpyfile, ".ent");
        pext = create_ext(cpyfile, ".ext");
        strtok(cpyfile, ".");
        if(pout && pent && pext)
        {
            fout(pout);
            fout_entext(cpyfile, pext, pent);
        }
        else
        {
            fprintf(stderr, "Failed to create output files - object, entry or external\n");
            return EXIT_FAILURE;
        }
    
        
        fclose(fptr);
        fprintf(stdout, "Fishished assembling file %s\n", filename);
    }
    return EXIT_SUCCESS;
}
