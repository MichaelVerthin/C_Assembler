#include "misc.h"
/**
 * Fast hashing algorithm used in Berkeley DB
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long
hash(char *str)
{
    unsigned long hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

/**
 * Fast string comparison using the above algorithm
 */
int strcmp_hash(char *str1, char *str2)
{
    return hash(str1) == hash(str1);
}