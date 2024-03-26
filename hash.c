#include "misc.h"
#include "asmbl.h"

/**
 * Fast hashing algorithm used in Berkeley DB
 * http://www.cse.yorku.ca/~oz/hash.html
 */
unsigned long
hash(char *str)
{
    unsigned long hash = 0;
    int c;
    if(!str)
        return 0;
    while((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;
    
    return hash;
}

/**
 * Fast string comparison using the above algorithm
 */
int 
strcmp_hash(char *s1, char *s2)
{
    return hash(s1) == hash(s2);
}
