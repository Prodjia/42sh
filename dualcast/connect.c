/*
** connect.c for connect.c in /home/arthur/delivery/PSU/PSU_2016_42sh
** 
** Made by Arthur Knoepflin
** Login   <arthur.knoepflin@epitech.eu>
** 
** Started on  Sat May  6 16:37:18 2017 Arthur Knoepflin
** Last update Mon May  8 17:14:15 2017 Arthur Knoepflin
*/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include "server.h"
#include "get_next_line.h"
#include "my.h"

static int	is_valid_code(char *code)
{
  int		i;

  if (my_strlen(code) != 4)
    return (0);
  i = 0;
  while (code[i])
    {
      if (code[i] < '0' || code[i] > '9')
	return (0);
      i += 1;
    }
  return (1);
}

static int	check_correct_passwd(t_socket sock)
{
  char		*ret;
  int		len;

  if ((ret = malloc(sizeof(char) * 3)) == NULL)
    return (0);
  if ((len = recv(sock, ret, 2, 0)) < 0)
    {
      free(ret);
      return (0);
    }
  ret[len] = '\0';
  if (!my_strcmp(ret, "OK"))
    {
      free(ret);
      return (1);
    }
  free(ret);
  return (0);
}

static int	send_passwd(t_socket sock)
{
  char		*resp;
  char		*code;

  my_printf("Entrer le code de session: ");
  if ((code = get_next_line(0)) == NULL)
    {
      close(sock);
      return (0);
    }
  if (!is_valid_code(code))
    {
      my_printf("Ceci n'est pas un code valide\n");
      close(sock);
      return (0);
    }
  if (send(sock, code, my_strlen(code), 0) < 0)
    {
      close(sock);
      return (0);
    }
  if (check_correct_passwd(sock))
    return (1);
  my_printf("Mauvais code de session\n");
  return (0);
}

t_socket		init_connect_dc(char *addr)
{
  t_socket		sock;
  t_sockaddr_in		sin;
  struct hostent	*hostinfo;

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    return (-1);
  if ((hostinfo = gethostbyname(addr)) == NULL)
    {
      dprintf(2, "Impossible de trouver %s\n", addr);
      return (-1);
    }
  sin.sin_addr = *(t_in_addr *) hostinfo->h_addr;
  sin.sin_port = htons(DUALCAST_PORT);
  sin.sin_family = AF_INET;
  if (connect(sock, (t_sockaddr *) &sin, sizeof(t_sockaddr)) == -1)
    {
      dprintf(2, "Impossible de se connecter sur %s\n", addr);
      return (-1);
    }
  if (send_passwd(sock))
    return (sock);
  close(sock);
  return (-1);
}
