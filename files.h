#ifndef FILE_H
#define FILE_H

#include "misc.h"
#include "asmbl.h"

int validate_filename(const char *filename);              /* Validates the filename extension */
FILE *create_ext(char *filename, char *ext);              /* Creates a new file with a specified extension */
void fout(FILE *const filename);                          /* Prints main memory image to file */
void fout_entext(char *filename, FILE *pext, FILE *pent); /* Prints .ent and .ext files */

#endif
