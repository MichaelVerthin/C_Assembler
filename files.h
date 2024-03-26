#ifndef FILE_H
#define FILE_H

#include "misc.h"
#include "asmbl.h"


int validate_filename(const char *filename);
FILE * create_ext(char *filename, char *ext);
void fout(FILE *const filename);
void fout_entext(char *filename, FILE *pext, FILE *pent);

#endif
