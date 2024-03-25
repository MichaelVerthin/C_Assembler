#ifndef LINE_H
#define LINE_H
#define LABEL_LEN 31 /* Maximum length of label */
#define LINE_LEN 80  /* Line length */
typedef struct line_s
{
    char *line;
    char *label;
} line_t;

int parse_line(line_t *);
char *is_label(char *);
int skipable_line(char *);
int is_macro(line_t *);

#endif