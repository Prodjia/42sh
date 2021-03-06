/*
** EPITECH PROJECT, 2018
** 42sh
** File description:
** line
*/

#include "my.h"
#include "shell.h"
#include <stdlib.h>
#include <string.h>

char *my_epurstr(char *str)
{
    char *ret = calloc(strlen(str) + 1, sizeof(char));
    int i[2] = {-1, 0};
    char b = 0;

    if (ret == NULL)
        return NULL;
    while (str[++(i[0])]) {
        if (!(i[0] == 0 && (str[i[0]] == ' ' || str[i[0]] == '\t')))
            ret[i[1]++] = ((str[i[0]] == '\t') ? ' ' : str[i[0]]);
        if (!b && (str[i[0]] == ' ' || str[i[0]] == '\t')) {
            while (str[i[0]] && (str[i[0]] == ' ' || str[i[0]] == '\t'))
                i[0] += 1;
            i[0] -= 1;
        }
        else if (str[i[0]] == b && b != 0)
            b = 0;
        else if ((str[i[0]] == '\'' || str[i[0]] == '"') && b == 0)
            b = str[i[0]];
    }
    ret[i[1]] = 0;
    free(str);
    return ret;
}

int count_links(char *str)
{
    int i = -1;
    int count = 0;
    char buf;

    while (str[++i]) {
        if (str[i] == '\\')
            i += !!(str[i + 1]);
        else if (str[i] == '\'' || str[i] == '"') {
            buf = str[i];
            i += 1;
            while (str[i] && str[i] != buf)
                i += (str[i] == '\\') + 1;
            if (str[i] == 0) {
                my_putchar_fd(buf, 2);
                return my_print_ret(": Invalid quotes.\n", -1);
            }
        }
        count += is_separator(str[i]);
    }
    return count;
}

void skip_and_copy_string(char *str, int *i, char *ret, int *t)
{
    char quote;

    quote = str[(*i)++];
    ret[(*t)++] = quote;
    while (str[*i] && str[*i] != quote) {
        if (str[*i] == '\\') {
            ret[(*t)++] = '\\';
            ret[(*t)++] = str[(*i) + 1];
            *i += !!(str[(*i) + 1]);
        }
        else
            ret[(*t)++] = str[*i];
        *i += 1;
    }
    ret[(*t)++] = str[(*i)];
}

void copy_escaped_char(char *str, int *i, char *ret, int *t)
{
    ret[(*t)++] = '\\';
    ret[(*t)++] = str[(*i) + 1];
    *i += !!(str[(*i) + 1]);
}

void my_epur_sep(char *ret, char *str, int *i, int *t)
{
    ret[(*t)++] = ' ';
    ret[(*t)++] = str[*i];
    if (str[*i] != ';' && str[*i + 1] == str[*i])
        ret[(*t)++] = str[++(*i)];
    ret[(*t)++] = ' ';
}

char *my_epurcommand(char *str)
{
    int i = count_links(str);
    char *ret = (i != -1) ? calloc(strlen(str) + 1 + i * 4, sizeof(char)) : 0;
    int t = 0;

    if (ret == NULL)
        return NULL;
    i = -1;
    while (str[++i])
        if (str[i] == '\\')
            copy_escaped_char(str, &i, ret, &t);
        else if (is_space(str[i])) {
            ret[t++] = ' ';
            while (is_space(str[++i]))
                ;
            i -= 1;
        }
        else if (str[i] == '"' || str[i] == '\'')
            skip_and_copy_string(str, &i, ret, &t);
        else if (is_separator(str[i]))
            my_epur_sep(ret, str, &i, &t);
        else
            ret[t++] = str[i];
    ret[t] = 0;
    free(str);
    return ret;
}
