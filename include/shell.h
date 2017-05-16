/*
1;4803;0c** shell.h for progammation shell in /home/nicolaspolomack/shell/PSU_2016_minishell1
**
** Made by Nicolas Polomack
** Login   <nicolas.polomack@epitech.eu>
**
** Started on  Tue Jan  3 19:13:06 2017 Nicolas Polomack
** Last update Tue May 16 19:10:27 2017 Nicolas Polomack
*/

#ifndef SHELL_H_
# define SHELL_H_

# include <termio.h>
# include <sys/stat.h>
# include "server.h"
# define RC_FILE ".42shrc"
# define HIST_FILE ".42sh_history"
# define ALIAS_FILE ".42sh_alias"

extern char	**environ;

typedef struct		s_command
{
  char			**av;
  char			link;
  char			*r_name;
  char			*r_type;
  char			*l_name;
  char			*l_type;
  int			count;
  struct s_command	*prev;
  struct s_command	*next;
}			t_command;

typedef struct		s_alias
{
  char			*alias;
  char			*command;
  struct s_alias	*next;
}			t_alias;

typedef struct		s_history
{
  char			**cmd;
  struct s_history	*next;
  struct s_history	*prev;
}			t_history;

typedef struct	s_hist_ctrl
{
  t_history	*first;
  t_history	*last;
  t_history	*cur;
  char		*cur_line;
}		t_hist_ctrl;

typedef struct	s_window
{
  struct termio	oterm;
  char		*clear;
  char		*end;
  char		*home;
  char		*forw;
  char		*backw;
  char		*upw;
  char		*downw;
  char		*left;
  char		*right;
  int		cur;
}		t_window;

/*
** This is the main structure that binds the shell together
**
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
typedef struct		s_shell
{
  int			prompt;
  char			**path;
  char			*home;
  char			*current;
  char			*line;
  char			**final;
  char			is_done;
  unsigned int		exit;
  char			*exit_str;
  char			*last;
  char			*prev;
  int			*fds;
  int			tty;
  int			ioctl;
  char			*cwd;
  t_alias		*alias;
  t_hist_ctrl		hist;
  t_command		*commands;
  t_command		*cur;
  t_window		w;
  int			is_comp;
}			t_shell;

unsigned int	count_args(char *);
int	get_next_arg(char *, char **, int);
char	**bufferize(char *, int);
char	*cat_path(char **, char *, int);
void	exec_child(t_shell *, int);
unsigned int	exec_command(char **, t_shell *);
unsigned int	exec_line(t_shell *, unsigned int);
void	parse_rc(t_shell *);
int	count_entries(char *);
int	get_next_entry(char *, char **, int);
char	**init_path(char *);
char	**set_default_path();
int	disp_env();
int	move_dir(char **, int, t_shell *);
unsigned int	exec_action(t_shell *, unsigned int);
unsigned int	process_command(t_shell *, int);
int	is_char_alpha(char *);
int	set_env(char *, char *);
int	unset_env(char **);
void	free_shell(t_shell *);
void	free_shell2(t_shell *);
void	free_commands(t_shell *);
int	my_print_err(char *);
int	my_print_ret(char *, int);
int	is_path(char *);
int	is_valid_path(char *);
void	diagnose_status(unsigned int);
int	check_wave(t_shell *);
void	add_hist(t_shell *);
void	my_print_command(t_shell *);
char	*detect_home(char *, char *);
char	*get_current(char *, char *);
int	exec_builtins(t_shell *, int, int *);
int	exec_builtins2(t_shell *, int, int *, int);
unsigned int	get_unsigned_int(char *);
int	is_line_empty(t_shell *);
int	init_shell(t_shell *, char **);
int	my_strlen_spe(char *, char);
void	init_aliases(t_shell *);
void	set_alias(t_shell *, char *);
void	reload_shell(t_shell *);
void	free_alias(t_shell *);
void	free_hist(t_shell *);
int	disp_alias(t_shell *, char *);
int	disp_all_alias(t_shell *);
int	add_alias(t_shell *, char *, char *);
void	save_alias(t_shell *);
void	write_alias(t_alias *, int);
void	my_print_fd(char *, int);
int	set_commands(t_shell *);
int	check_access(char **, t_shell *);
int	check_env_error(char *);
int	check_exit(t_shell *, int);
int	compare_stats(struct stat *);
void	check_exec(t_shell *, int, int *);
void	exec_piped_command(char *, t_command *, int[2], t_shell *);

/*
** alias/alias.c
*/
char	*construct_alias(char **);
char	*get_alias_cmd(t_shell *, char *);

/*
** alias/edit.c
*/
int	parse_alias(t_shell *);

/*
** alias/unalias.c
*/
int	unalias(t_shell *, char **);

/*
** config.c
*/

int	config_http(t_config *);

/*
** history.c
*/
void	save_history(t_shell *);
int	disp_hist(t_shell *);
void	add_hist_elem(t_shell *, char *);
void	init_history(t_shell *);

/*
** history2.c
*/
int	parse_history(t_shell *, int);

/*
** exec/exec2.c
*/
unsigned int	exec_redirected_builtins(t_shell *, int, int *, int[2]);
void		quick_exec(t_shell *, char *);

/*
** exec/close.c
*/
int	close_pipes(int *);

/*
** dualcast.c
*/

int	launch_dualcast(t_shell *, int);

/*
** dualcast/connect.c
*/

t_socket	init_connect_dc(char *);

/*
** dualcast/wait_connection.c
*/

t_socket	wait_connection(t_socket, fd_set *, char *);

/*
** dualcast/init_dualcast.c
*/

int	init_dualcast(t_socket *);

/*
** dualcast/get_password.c
*/
char	*get_password();

/*
** launch_config.c
*/

int	launch_config(t_shell *);

/*
** redirects.c
*/
int	setup_right_redirect(t_command *, int *, int);
int	setup_left_redirect(char *, int);
int	check_redirects(t_command *, t_command *);
int	prepare_redirect(t_command *, char **, char **, int);
int	set_redirects(t_shell *);

/*
** buffer.c
*/
int	buffer_input(char *, int[2]);

/*
** parse/line.c
*/
char	*my_epurstr(char *);
char	*my_epurcommand(char *);

/*
** parse/error.c
*/
int	check_error(t_shell *);

/*
** exec/pipe.c
*/
int	exec_pipeline(t_shell *);
int	father_action(t_command **, int *, int *, t_shell *);
void	exec_piped_child(int, t_command *, int[2], t_shell *);

/*
** exec/setup.c
*/
void	init_redirect(t_command *, int *, int *, int *);
void	setup_exec(t_command *, int *, int);
void	skip_commands(t_command **, unsigned char);

/*
** globbing/globbing.c
*/
int	parse_vars(t_shell *);

/*
** globbing/home.c
*/
void	replace_home(t_shell *);

/*
** echo.c
*/
int	echo_term(char **);

/*
** is.c
*/
int	is_redirect(char *);
int	is_right_redirect(char *);
int	is_left_redirect(char *);
int	is_dir(char *);
int	is_builtin(char *);

/*
** is2.c
*/
int	is_separator(char);
int	is_space(char);
int	is_delimiter(char);
int	is_alphanum(char);

/*
** char.c
*/
void	insert_char(char **, char);
void	insert_int(int **, int);

/*
** print.c
*/
int	ret_error(t_shell *, char *);

/*
** prompt.c
*/
char	*get_hostname(void);
void	print_prompt(t_shell *);
void	get_prompt(t_shell *);

/*
** exit.c
*/
int	set_error(t_shell *, int);

/*
** init.c
*/

void	set_raw(struct termio *oterm);
void	init(t_shell *);
void	init_prompt(t_shell *);

/*
** misc.c
*/
void	set_raw();
char	get_input();
void	handle_error(char *);

/*
** prompt/get_cur_branch.c
*/

char	*show_cur_branch();

/*
** prompt/mechanics/char.c
*/
void	insert_char_cur(char **, char, int);
void	delete_char(char **, int);

/*
** prompt/mechanics/sauv.c
*/

void	sauv_prompt(t_shell *);

/*
** prompt/mechanics/load_file.c
*/

void	free_tab(char **);
char	**load_file(int);

/*
** prompt/mechanics/actions.c
*/
void	remove_char(t_shell *);
void	add_char(t_shell *, char);
void	move_cursor(t_shell *, char);
void	clear_term(t_shell *);
void	pos_cursor(t_shell *);

/*
** prompt/mechanics/cursor.c
*/
void	buffer_seq(t_shell *, char **, int *, char);
void	move_forw(t_shell *);
void	move_backw(t_shell *);
void	move_upw(t_shell *);
void	move_downw(t_shell *);

/*
** prompt/mechanics/advanced.c
*/
void	move_home(t_shell *);
void	move_end(t_shell *);

/*
** prompt/mechanics/fct.c
*/
void	get_cur_fcts(void (*[6])(t_shell *));

/*
** prompt/mechanics/prompt.c
*/
void	prompt_line(t_shell *);

#endif /* !SHELL_H_ */
