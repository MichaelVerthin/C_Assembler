#pragma once
struct line_s
{
    char *line;
    char *first;
    char *second;
    char *third;
    char *fourth;
    char *fifth;
};
typedef struct line_s line_t;

line_t *parse_line(line_t *);