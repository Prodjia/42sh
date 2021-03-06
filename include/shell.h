/*
** EPITECH PROJECT, 2018
** 42sh
** File description:
** shell
*/

#pragma once

#include "hmap.h"
#include "server.h"
#include "str.h"
#include "vec.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>

#define RC_FILE ".42shrc"
#define HIST_FILE ".42sh_history"
#define ALIAS_FILE ".42sh_alias"

extern char **environ;

typedef struct command_s {
    char **av;
    char link;
    char *r_name;
    char *r_type;
    char *l_name;
    char *l_type;
    int count;
    struct command_s *prev;
    struct command_s *next;
} command_t;

typedef struct alias_s {
    char *alias;
    char *command;
} alias_t;

typedef struct subst_s {
    int idx;
    char **list;
} subst_t;

typedef struct hist_ctrl_s {
    vec_t *arr;
    long cur;
    char *cur_line;
} hist_ctrl_t;

typedef struct window_s {
    struct termios oterm;
    char *smkx;
    char *clear;
    char *end;
    char *home;
    char *forw;
    char *backw;
    char *upw;
    char *downw;
    char *left;
    char *right;
    int cur;
} window_t;

typedef struct config_s {
    char *rc_file;
    char *alias_file;
    char *history_file;
    char *prompt;
} config_t;

/*
** This is the main structure that binds the shell together
**
** av: av give in the main (for var $0, $1, ...)
** script: is launch from script
** env: the environement
** path: the current path
** home: the current home, for prompt & cd builtin
** current: the current working directory, for prompt
** line: raw command input from user
** final: parsed command from line
** hist: last parsed command executed
** hist_args: arg count from hist
** exit: last exit status, for exit builtin
** exit_str: last exit status converted to str, for prompt
** last: used for alias processing, avoids loops
** prev: last working directory, for cd builtin
**
*/
typedef struct shell_s {
    char **av;
    int script;
    int prompt;
    char **path;
    char *home;
    char **vars;
    char *current;
    char *line;
    char **final;
    char is_done;
    unsigned int exit_code;
    char *exit_str;
    char *last;
    char *prev;
    int *fds;
    int tty;
    int ioctl;
    char *cwd;
    pid_t pgid;
    hmap_t *alias;
    subst_t subst;
    hist_ctrl_t hist;
    command_t *commands;
    command_t *cur;
    window_t w;
    config_t config;
    int is_comp;
} shell_t;

unsigned int count_args(char *);
int get_next_arg(char *, char **, int);
char **bufferize(char *, int);
char *cat_path(char **, char *, int);
void exec_child(shell_t *);
unsigned int exec_command(char **, shell_t *);
unsigned int exec_line(shell_t *, unsigned int);
void parse_rc(shell_t *);
int count_entries(char *);
int get_next_entry(char *, char **, int);
char **init_path(char *);
char **set_default_path();
int disp_env();
int move_dir(char **, int, shell_t *);
unsigned int exec_action(shell_t *, unsigned int);
unsigned int process_command(shell_t *, int);
int is_char_alpha(char *);
int set_env(char *, char *);
int unset_env(char **);
void free_shell(shell_t *);
void free_shell2(shell_t *);
void free_commands(shell_t *);
int my_print_err(char *);
int my_print_ret(char *, int);
int is_path(char *);
int is_valid_path(char *);
void diagnose_status(unsigned int);
int check_wave(shell_t *);
void add_hist(shell_t *);
void my_print_command(shell_t *);
char *detect_home(char *, char *);
char *get_current(char *, char *);
int indexof_builtin(char *);
int exec_builtins(shell_t *, int, int *);
int exec_builtins2(shell_t *, int, int *, int);
unsigned int get_unsigned_int(char *);
int is_line_empty(char *);
int init_shell(shell_t *);
int my_strlen_spe(char *, char);
void init_aliases(shell_t *);
void set_alias(shell_t *, char *);
void reload_shell(shell_t *);
void free_alias(shell_t *);
void free_hist(shell_t *);
int disp_alias(shell_t *, char *);
int disp_all_alias(shell_t *);
int add_alias(shell_t *, char *, char *);
void save_alias(shell_t *);
void my_print_fd(char *, int);
int set_commands(shell_t *);
int check_access(char **, shell_t *);
int check_env_error(char *, char *);
int check_exit(shell_t *, int);
int compare_stats(struct stat *);
void check_exec(shell_t *, int, int *);
void exec_piped_command(char *, command_t *, int[2], shell_t *);
char *format_arg(char *);

/*
** alias/alias.c
*/
char *construct_alias(char **);
char *get_alias_cmd(shell_t *, char *);

/*
** alias/edit.c
*/
int parse_alias(shell_t *);

/*
** alias/unalias.c
*/
int unalias(shell_t *, char **);

/*
** alias/loop.c
*/
void free_subst(shell_t *);
int detect_loop(shell_t *, char *, int);

/*
** comment.c
*/

int clear_comment(shell_t *);

/*
** config.c
*/

int config_http(t_config *);

/*
** history.c
*/
void save_history(shell_t *);
int disp_hist(shell_t *, int);
void add_hist_elem(shell_t *, char *);
void init_history(shell_t *);
void skip_string(char *, int *);

/*
** history2.c
*/
int parse_history(shell_t *, int);

/*
** magic/magic.c
*/
int magic(shell_t *);

/*
** magic/construct.c
*/
char *sanitize(char *, int);
char *construct_magic(char **);

/*
** exec/exec2.c
*/
unsigned int exec_redirected_builtins(shell_t *, int, int *, int[2]);
void quick_exec(shell_t *, char *);

/*
** exec/close.c
*/
int close_pipes(int *);

/*
** dualcast.c
*/

int launch_dualcast(shell_t *, int);

/*
** dualcast/wait_connection.c
*/

t_socket wait_connection(t_socket, fd_set *, char *);

/*
** dualcast/init_dualcast.c
*/

int init_dualcast(t_socket *);

/*
** dualcast/get_password.c
*/
char *get_password();

/*
** launch_config.c
*/

int launch_config(shell_t *);

/*
** redirects.c
*/
int setup_right_redirect(command_t *, int *, int);
int setup_left_redirect(char *, int);
int check_redirects(command_t *, command_t *);
int prepare_redirect(command_t *, char **, char **, int);
int set_redirects(shell_t *);

/*
** buffer.c
*/
int buffer_input(char *, int[2]);

/*
** parse/line.c
*/
char *my_epurstr(char *);
char *my_epurcommand(char *);

/*
** parse/error.c
*/
int check_error(shell_t *);

/*
** exec/pipe.c
*/
int exec_pipeline(shell_t *);
int father_action(command_t **, int *, int *, shell_t *);
void exec_piped_child(int, command_t *, int[2], shell_t *);

/*
** exec/tmp.c
*/
void tmp_file(shell_t *);

/*
** exec/setup.c
*/
void init_redirect(command_t *, int *, int *, int *);
void setup_exec(command_t *, int *, int);
void skip_commands(command_t **, unsigned char);
void set_fground(shell_t *);

/*
** globbing/globbing.c
*/
int parse_vars(shell_t *);

/*
** globbing/.c
*/
void replace_home(shell_t *);

/*
** globbing/stars.c
*/
int parse_stars(shell_t *);

/*
** vars.c
*/
void add_var(shell_t *, char *, char *);

/*
** echo.c
*/
char get_escaped_char(char);
int echo_term(char **);

/*
** is.c
*/
int is_redirect(char *);
int is_right_redirect(char *);
int is_left_redirect(char *);
int is_dir(char *);
int is_builtin(char *);
int is_to_fork(char);

/*
** is2.c
*/
int is_separator(char);
int is_space(char);
int is_delimiter(char);
int is_alphanum(char);
int is_num(char *);

/*
** builtins/set.c
*/
int set(shell_t *, int);

/*
** builtins_init.c
*/
int nb_built(const char **);

/*
** builtins/unset.c
*/
int unset(shell_t *, int);

/*
** char.c
*/
void insert_char(char **, char);
void insert_int(int **, int);

/*
** print.c
*/
int ret_error(shell_t *, char *);

/*
** prompt.c
*/
char *get_hostname(void);
void print_prompt(shell_t *);
void get_prompt(shell_t *);

/*
** exit.c
*/
int set_error(shell_t *, int);

/*
** vars.c
*/
char *get_var(shell_t *, char *);
void init_vars(shell_t *);

/*
** init.c
*/

void set_raw(struct termios *);
void init(shell_t *);
void init_prompt(shell_t *);

/*
** misc.c
*/
void set_raw();
char get_input();
void handle_error(char *);

/*
** prompt/get_cur_branch.c
*/

char *show_cur_branch();

/*
** prompt/mechanics/char.c
*/
void insert_char_cur(char **, char, int);
void delete_char(char **, int);

/*
** prompt/mechanics/sauv.c
*/

void sauv_prompt(shell_t *);

/*
** prompt/mechanics/load_file.c
*/

void free_tab(char **);
char **load_file(int);

/*
** prompt/mechanics/actions.c
*/
void remove_char(shell_t *);
void add_char(shell_t *, char);
void move_cursor(shell_t *, char);
void clear_term(shell_t *);
void pos_cursor(shell_t *);

/*
** prompt/mechanics/cursor.c
*/
void buffer_seq(shell_t *, char **, int *, char);
void move_forw(shell_t *);
void move_backw(shell_t *);
void move_upw(shell_t *);
void move_downw(shell_t *);

/*
** prompt/mechanics/advanced.c
*/
void move_home(shell_t *);
void move_end(shell_t *);
void set_hist_line(shell_t *);
void suppress_line(shell_t *);

/*
** prompt/mechanics/fct.c
*/
void get_cur_fcts(void (*[6])(shell_t *));

/*
** prompt/mechanics/prompt.c
*/
void prompt_line(shell_t *);
