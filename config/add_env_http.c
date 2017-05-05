/*
** add_env_http.c for add_env_http in /home/arthur/Documents/Sock
** 
** Made by Arthur Knoepflin
** Login   <arthur.knoepflin@epitech.eu>
** 
** Started on  Mon Apr 24 23:36:51 2017 Arthur Knoepflin
** Last update Fri May  5 02:32:00 2017 Nicolas Polomack
*/

#include <stdlib.h>
#include "shell.h"
#include "server.h"
#include "my.h"

void	add_env_http(t_socket client, char **arg)
{
  char	*decode;

  if (getenv(arg[3]))
    {
      write_client(client, ERROR_RESP);
      return ;
    }
  if (nb_args(arg) == 6)
    {
      if ((decode = malloc(sizeof(char) *
			   (my_strlen(arg[5]) + 1))) == NULL)
	return ;
      urldecode(arg[5], decode);
      set_env(arg[3], decode);
      free(decode);
    }
  else if (nb_args(arg) == 5)
    set_env(arg[3], "");
  write_client(client, BASE_RESP);
  write_client(client, "add_env_ok");
}

void	del_env_http(t_socket client, char **arg)
{
  unsetenv(arg[3]);
  write_client(client, BASE_RESP);
  write_client(client, "del_env_ok");
}

void	update_env_http(t_socket client, char **arg)
{
  int	idx;
  char	*decode;
  char	*new;

  if (nb_args(arg) == 6)
    {
      if ((decode = malloc(sizeof(char) *
			   (my_strlen(arg[5]) + 1))) == NULL)
	return ;
      urldecode(arg[5], decode);
      set_env(arg[3], decode);
    }
  write_client(client, BASE_RESP);
  write_client(client, "update_env_ok");
}
