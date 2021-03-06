/*
** EPITECH PROJECT, 2018
** 42sh
** File description:
** init
*/

#include "shell.h"
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <term.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

void set_raw(struct termios *oterm)
{
    struct termios nterm;

    nterm = *oterm;
    nterm.c_lflag &= ~(ICANON | ECHO);
    nterm.c_cc[VTIME] = 0;
    nterm.c_cc[VMIN] = 0;
    if (tcsetattr(0, TCSANOW, &nterm) == -1)
        handle_error("tcsetattr");
}

int init_shell(shell_t *shell)
{
    srand(getpid() * time(NULL));
    shell->home = getenv("HOME");
    shell->current = calloc(512, sizeof(char));
    if (shell->current == 0)
        return -1;
    shell->current = get_current(shell->current, shell->home);
    shell->exit_code = 0;
    shell->is_comp = 0;
    shell->last = NULL;
    shell->prev = NULL;
    shell->exit_str = NULL;
    shell->fds = NULL;
    shell->is_done = 0;
    init_history(shell);
    init_aliases(shell);
    parse_rc(shell);
    init_vars(shell);
    get_prompt(shell);
    shell->path = init_path(getenv("PATH"));
    shell->path = (shell->path) ? shell->path : set_default_path();
    init(shell);
    return 0;
}

void init(shell_t *shell)
{
    shell->tty = isatty(0);
    if (shell->tty) {
        if (getenv("TERM") == NULL || setupterm(NULL, 0, NULL) == ERR) {
            shell->ioctl = 0;
            return;
        }
        shell->ioctl = tcgetattr(0, &shell->w.oterm) + 1;
        shell->w.smkx = tigetstr("smkx");
        shell->w.clear = tigetstr("clear");
        shell->w.end = tigetstr("kend");
        shell->w.home = tigetstr("khome");
        shell->w.forw = tigetstr("cuf1");
        shell->w.backw = tigetstr("cub1");
        shell->w.upw = tigetstr("kcuu1");
        shell->w.downw = tigetstr("kcud1");
        shell->w.left = tigetstr("kcub1");
        shell->w.right = tigetstr("kcuf1");
        shell->line = NULL;
    }
}

void init_prompt(shell_t *shell)
{
    shell->hist.cur = -1;
    shell->hist.cur_line = NULL;
    if (shell->tty) {
        if (shell->ioctl) {
            printf("%s", shell->w.smkx);
            fflush(stdout);
        }
        get_prompt(shell);
        sauv_prompt(shell);
        print_prompt(shell);
    }
}
