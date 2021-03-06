/*
** EPITECH PROJECT, 2018
** 42sh
** File description:
** path
*/

#include "get_next_line.h"
#include "my.h"
#include "shell.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

char *cat_path(char **path, char *final, int i)
{
    char *str;
    char *dir;

    if (i == -1 || i == -2)
        dir = strdup((i == -2) ? "/bin" : "/usr/bin");
    else if (i >= 0)
        dir = path[i];
    else
        return NULL;
    str = calloc(strlen(dir) + strlen(final) + 2, sizeof(char));
    if (str == NULL)
        handle_error("calloc");
    *str = 0;
    str = strcat(str, dir);
    if (dir[strlen(dir)] != '/')
        str[strlen(dir)] = '/';
    str[strlen(dir) + 1] = 0;
    str = strcat(str, final);
    if (i < 0)
        free(dir);
    return str;
}

int is_path(char *str)
{
    int i;

    i = -1;
    while (str[++i] != 0)
        if (str[i] == '/')
            return 1;
    return 0;
}

char **init_path(char *str)
{
    char **path;

    if (str == NULL)
        return NULL;
    path = my_split(str, ":");
    if (path == NULL)
        handle_error("calloc");
    return path;
}

void set_path(shell_t *shell, char *path)
{
    int i = -1;
    int obj = my_strlen_spe(path, '=');
    char *name = calloc(obj + 1, sizeof(char));
    char *entry;

    (void)(shell);
    if (name == NULL)
        handle_error("calloc");
    while (++i < obj)
        name[i] = path[i];
    name[i] = 0;
    i = obj + 1;
    obj = my_strlen_spe(path + i, '\0');
    entry = calloc(obj + 1, sizeof(char));
    if (entry == NULL)
        handle_error("calloc");
    obj = i;
    i = -1;
    while ((path + obj)[++i] != 0)
        entry[i] = (path + obj)[i];
    entry[i] = 0;
    setenv(name, entry, 1);
    free(name);
    free(entry);
}

void parse_rc(shell_t *shell)
{
    int fd;
    char *path;

    if (shell->home == NULL)
        return;
    path = calloc(512, sizeof(char));
    if (path == NULL)
        handle_error("calloc");
    path[0] = 0;
    path = strcat(path, shell->home);
    if (shell->home[strlen(shell->home)] != '/')
        path[strlen(shell->home)] = '/';
    path[strlen(shell->home) + 1] = 0;
    path = strcat(path, RC_FILE);
    if ((fd = open(path, O_RDONLY)) == -1)
        return;
    free(path);
    while ((path = get_next_line(fd)) != NULL) {
        if (is_valid_path(path))
            set_path(shell, path);
        free(path);
    }
    close(fd);
}
