#include <string.h>
#include <ctype.h>
#include <assert.h> /* DEBUG */
#include "line.h"
#include "asmbl.h"
#include "misc.h"
#include "symbols.h"

char *strsub(char *, size_t, char *);
char *is_label(char *);
int is_macro(line_t *);
int parse_line(line_t *pLINE)
{
    char *label;
    /***********************************************\
                        LABELS
    \***********************************************/
    if ((label = is_label(pLINE->line)))
        pLINE->label = label;
    else
        pLINE->label = NULL;
    /* strtok label from line, thus taking only the substring */
    if (pLINE->label)
    {
        strtok(pLINE->line, " ");
        pLINE->line = strtok(NULL, "\0");
    }
    /************************************************/

    /***********************************************\
                        MACROS
    \***********************************************/
    if (is_macro(pLINE))
    {
        if (pLINE->label)
        {
            ERROR_MSG("No label is allowed on a macro sentence\nAborting")
            return EXIT_FAILURE;
        }
        return PARSED_MACRO;
    }
    /************************************************/
    return EXIT_FAILURE;
}

int is_macro(line_t *pLINE)
{
    char *ch = pLINE->line;
    char *name, *data_s;
    int data;

    /* trim whitespaces from start */
    while (isspace(*ch++))
        ;
    ch = strtok(ch - 1, " ");
    /* .define has been identified */
    if (strcmp_hash(ch, ".define"))
    {
        symbol_t *macro = init_macro();
        name = strtok(NULL, "=");
        if (!macro)
        {
            ERROR_MSG("Failed to allocate memory for macro_t\nAborting...")
            return FALSE;
        }
        /* Extracting the name of the macro */
        if ((name = clear_str(name)) != NULL)
            macro->symbol->macro->name = name;
        else
        {
            ERROR_MSG("Error Extracting macro name\nAborting...")
            destroy_macro(macro);
            return FALSE;
        }
        /* Extracting the data of the macro */
        data_s = clear_str(strtok(NULL, "\0"));
        data = is_num(data_s);
        if (data != _12BIT_MIN)
            macro->symbol->macro->data = data;
        else
        {
            ERROR_MSG("Error extracting macro data\nAborting...")
            destroy_macro(macro);
            return FALSE;
        }
        /* Pointing the line object to the macro object */
        pLINE->parsed = macro;
        return TRUE;
    }

    return FALSE;
}

/**
 * Check if line contains a label, returns the label name if exists.
 */
char *
is_label(char *line)
{
    char *st, *ch, *label;
    ch = line;
    while (isspace(*ch++))
        st = ch - 1;
    if (isalpha(*st))
    {
        while (isgraph(*ch) && *ch != ':')
            ch++;
        /* Reached the end of the label title */
        if (*ch == ':' && *(ch + 1) == ' ')
        {
            if (ch - st > LABEL_LEN)
            {
                ERROR_MSG("Label is too long.")
                SAFE_FREE(label);
                return NULL;
            }
            /* Label is a reserved word */
            if (is_reserved(label))
            {
                ERROR_MSG("Label name is a reserved word")
                SAFE_FREE(label);
                return NULL;
            }
            return label;
        }
    }
    return NULL;
}

/* Is the line completely blank? */
int is_whitespace(char *line)
{
    char *ch = line;
    while (*line != '\0')
    {
        if (!isspace(*line++))
            return FALSE;
    }
    return TRUE;
}

/* Is the line a comment? */
is_comment(char *line)
{
    return (*line == ';');
}

/* Is the line worth parsing at all? */
int skipable_line(char *line)
{
    return (is_comment(line) || is_whitespace(line));
}

/**
 * Allocates a new string which contains the characters
 * from index ipos up to len characters, adds the null terminator at the end of sub.
 * Returns the new substring; on failure returns NULL.
 * NOTE: The function uses dynamically allocated memory,
 * needs to be free()'ed with FREE_SAFE or LINE_FREE()
 */
char *
strsub(char *pos, size_t len, char *str)
{
    int c = 0;
    char *sub = (char *)malloc(sizeof(char) * len);
    if (!sub)
        return NULL;
    while (c < len)
    {
        sub[c] = *pos++;
        c++;
    }
    sub[c] = '\0';

    return sub;
}