#include <stdio.h>
#include "asmbl.h"
#include "misc.h"



/**
 * Validates a number is in boundaries of 12 bit words.
 */
int 
is_valid(long num)
{
    return num <= _12BIT_MAX && num >= _12BIT_MIN;
}

/**
 * Convert binary string to its equivalence integer number.
 * support 12 bit binary
 */
int 
conv_bd(int bin_num)
{
    int rem, i = 0;
    int dec_num = 0;
    if (!is_valid(bin_num))
    {
        fprintf(stderr, "Number exceeds word boundaries");
        return -1;
    }
    while (bin_num > 0)
    {
        rem = bin_num % 10;
        bin_num /= 10;
        dec_num += rem << i;
    }
    return dec_num;
}

/**
 * Convert integer number to its equivalence binary *string*
 * support 12 bit binary
 */
int 
conv_db(int dec_num)
{
    if (!is_valid(dec_num))
    {
        fprintf(stderr, "Number exceeds word boundaries");
        return -1;
    }
    while(dec_num > 0)
    {
        break;
    }
    return dec_num;
}