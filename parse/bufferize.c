/*
** bufferize.c for minishell1 in /home/nicolaspolomack/shell/PSU_2016_minishell1
**
** Made by Nicolas Polomack
** Login   <nicolas.polomack@epitech.eu>
**
** Started on  Mon Jan  9 10:55:14 2017 Nicolas Polomack
** Last update Thu May 18 01:34:19 2017 Nicolas Polomack
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"
#include "my.h"
#include "get_next_line.h"

char	**bufferize(char *av, int n)
{
  int	i;
  int	args;
  char	**final;
  char	*str;

  args = 0;
  i = -1;
  final = my_split(av, " ");
  return (final);
}

int	count_link(char **final)
{
  int	i;

  i = 0;
  while (final[i] && (my_strcmp(final[i], "|") &&
		      my_strcmp(final[i], ";") &&
		      my_strcmp(final[i], "||") &&
		      my_strcmp(final[i], "&&")))
    i += 1;
  return (i);
}

void	prepare_link(t_shell *shell, t_command *elem, int i, int last)
{
  if (shell->final[i] == NULL)
    elem->link = '0';
  else if (my_strcmp(shell->final[i], "&&") == 0)
    elem->link = 'e';
  else if (my_strcmp(shell->final[i], "||") == 0)
    elem->link = 'o';
  else
    elem->link = shell->final[i][0];
  shell->final[i] = NULL;
  elem->av = my_strarrdup(shell->final + last);
  (elem->link != '0') ? insert_char(&(shell->final[i]), elem->link) : NULL;
  elem->count = -1;
}

int		set_commands(t_shell *shell)
{
  t_command	*elem;
  t_command	*head;
  int		i;
  int		last;

  shell->commands = NULL;
  head = NULL;
  i = -1;
  while (shell->final[++i])
    {
      if ((elem = malloc(sizeof(*elem))) == NULL)
	return (-1);
      elem->next = NULL;
      (head) ? (head->next = elem) : (shell->commands = elem);
      elem->prev = head;
      last = i;
      if ((i += count_link(shell->final + i)) == last)
	return (ret_error(shell, "Invalid null command.\n"));
      prepare_link(shell, elem, i, last);
      i -= (elem->link == '0');
      head = elem;
    }
  return (0);
}
