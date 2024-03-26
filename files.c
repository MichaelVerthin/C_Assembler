#include <stdio.h>
#include <string.h>
#include "line.h"
#include "files.h"
#include "opcodes.h"
#include "globals.h"
#include "asmbl.h"


void print_bin(FILE *filename, int bin);

/**
 * Validates the file extention is .as
 */
int
validate_filename(const char *filename)
{
    char* ext, file[30];
    if(!filename)
        return ERROR;
    strcpy(file, filename);
    strtok(file, ".");
    ext = strtok(NULL, "\0");
    if(strcmp_hash(ext, "as"))
        return TRUE;
    return FALSE;
}

/**
 * Create new file that has ext extention
 */
FILE *
create_ext(char *filename, char *ext)
{
    if(!filename || !ext)
        return NULL;
    strtok(filename, ".");
    strcat(filename, ext);
    return fopen(filename, "w");
}

/**
 * Convert integer value to its encoded 4 bit base.
 */
void
convertbits(FILE * const filename, int bin)
{
    int mask = 3, i = 0, sym;
    char symbols[7];
    if(!filename)
        return;
    while(i < 7)
    {
        sym = bin & mask;
        switch (sym)
        {
        case 0:
            symbols[i] = '*';
            break;
        case 1:
            symbols[i] = '#';
            break;
        case 2:
            symbols[i] = '%';
            break;
        case 3:
            symbols[i] = '!';
            break;
        default:
            break;
        }
    i++;
    bin >>= 2;
    }
    for(i = 0; i < 7; i++)
        fprintf(filename, "%c", symbols[6 - i]);
}

/**
 * main memory image function printer
 */
void
fout(FILE *const filename)
{
    int i;
    if(!filename)
        return;
    fprintf(filename, "%4d\t%d\n", IC, DC);
    for (i = 0; i < IC; i++)
    {
        fprintf(filename, "%04d\t", i + 100);
        print_bin(filename, (int) instruction_arr[i].reg);
        convertbits(filename, instruction_arr[i].reg);
        fprintf(filename, "\n");
    }
    for(i = 0; i < DC; i++)
    {
        fprintf(filename, "%04d\t", IC + 100 + i);
        print_bin(filename, (int) data_arr[i].reg);
        convertbits(filename, data_arr[i].reg);
        fprintf(filename, "\n");
    }
    fclose(filename);
}

void
print_bin(FILE *filename, signed bin)
{
    int mask = 1, i = 0;
    int bins[14] = {0};
    while(i < 14)
    {
        if (bin & mask)
        {
            bins[i] = 1;
        }
        else
            bins[i] = 0;
        bin >>= 1;
        i++;
    }
    for(i = 0; i < 14; i++)
        printf("%d", bins[13-i]);
    printf("\n");
}


void
fout_entext(char *filename, FILE *pext, FILE *pent)
{
    size_t i, ent_size, ext_size;
    if(!filename || !pext || !pent)
        return;
    for (i = 0; i < IC; i++)
    {
        if(instruction_arr[i].type == SYMBOL_EXTERNAL)
            fprintf(pext, "%s\t%0ld\n", instruction_arr[i].name, i + 100 + 1);
    }
    for(i = 0; i < 2; i++)
    {
        fprintf(pent, "%s\t%d\n", entries[i].name, entries[i].reg);
    }

    fseek(pent, 0L, SEEK_END);
    ent_size = ftell(pent);
    fseek(pext, 0L, SEEK_END);
    ext_size = ftell(pext);

    if(!ext_size)
        remove(strcat(filename, ".ext"));
    if(!ent_size)
        remove(strcat(filename, ".ent"));
    fclose(pext);
    fclose(pent);
    return;
}
WORD_T instruction_arr[INITIAL_IC_ARR];
WORD_T data_arr[INITIAL_DC_ARR];
WORD_T entries[20];
