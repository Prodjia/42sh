/*
** check.c for minishell1 in /home/nicolaspolomack/shell/PSU_2016_minishell1
** 
** Made by Nicolas Polomack
** Login   <nicolas.polomack@epitech.eu>
** 
** Started on  Mon Jan  9 10:55:14 2017 Nicolas Polomack
** Last update Thu Apr 27 15:44:38 2017 Nicolas Polomack
*/

#include <stdlib.h>
#include "my.h"
#include "shell.h"

int	skip_quotes(char *str, int i, char *ret, int *t)
{
  char	buf;

  buf = str[i];
  ret[*t] = buf;
  i += 1;
  *t += 1;
  while (str[i] && str[i] != buf)
    {
      ret[*t] = str[i];
      *t += 1;
      i += 1;
    }
  if (str[i] == 0)
    {
      my_putchar_fd(buf, 2);
      return (my_print_ret(": Invalid quotes.\n", -1));
    }
  ret[*t] = buf;
  return (i);
}

char	*my_epurstr(char *str)
{
  char	*ret;
  int	i[2];
  char	b;

  i[0] = -1 + (i[1] = 0) + (b = 0);
  if ((ret = malloc(my_strlen(str) + 1)) == NULL)
    return (NULL);
  while (str[++(i[0])])
    {
      ret[i[1]] = str[i[0]];
      if (!b && str[i[0]] == ' ' || str[i[0]] == '\t')
	{
	  while (str[i[0]] && (str[i[0]] == ' ' || str[i[0]] == '\t'))
	    i[0] += 1;
	  i[0] -= 1;
	}
      else if (str[i[0]] == b && b != 0)
	b = 0;
      else if (str[i[0]] == '\'' || str[i[0]] == '"' && b == 0)
	b = str[i[0]];
      i[1] += 1;
    }
  ret[i[1]] = 0;
  free(str);
  return (ret);
}

int	count_links(char *str)
{
  int	i;
  int	count;
  char	buf;

  i = -1;
  count = 0;
  while (str[++i])
    {
      if (str[i] == '\'' || str[i] == '"')
	{
	  buf = str[i];
	  i += 1;
	  while (str[i] && str[i] != buf)
	    i += 1;
	  if (str[i] == 0)
	    {
	      my_putchar_fd(buf, 2);
	      return (my_print_ret(": Invalid quotes.\n", -1));
	    }
	}
      count += is_separator(str[i]);
    }
  return (count);
}

char	*my_epurcommand(char *str)
{
  char	*ret;
  int	i;
  int	t;

  if ((i = count_links(str)) == -1 ||
      (ret = malloc(my_strlen(str) + 1 + i * 4)) == NULL)
    return (NULL);
  i = -1 + (t = 0);
  while (str[++i])
    {
      if (is_separator(str[i]) && (i > 0 && str[i - 1] != ' '))
	ret[t++] = ' ';
      ret[t] = str[i];
      if ((str[i] == '<' || str[i] == '>' || str[i] == '|' || str[i] == '&') &&
	  str[i] == str[i + 1])
	ret[++t] = str[++i];
      if (is_separator(str[i]) && (str[i + 1] != ' '))
	ret[++t] = ' ';
      if (str[i] == '\'' || str[i] == '"')
	if ((i = skip_quotes(str, i, ret, &t)) == -1)
	  return (NULL);
      t += 1;
    }
  free(str);
  ret[t] = 0;
  return (ret);
}
