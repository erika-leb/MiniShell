/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:19:30 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/14 11:43:10 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_is_option(char *opt)
{
	int	i;

	i = 1;
	if (opt[0] != '-')
		return (FALSE);
	while (opt[i])
	{
		if (opt[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

static void	ft_fill_buf(int *i, int *k, char **cmd, char *buf)
{
	int	j;

	j = 0;
	while (cmd[*i][j])
		buf[(*k)++] = cmd[*i][j++];
	(*i)++;
}

static void	write_error(int f, t_gc *gc)
{
	if (f == -1)
	{
		perror("minishell: echo: write error");
		(gc_cleanup(gc), free_std(), exit(1));
	}
}

static void	no_arg(int *n, char **cmd, t_gc *gc)
{
	if (!cmd[1])
	{
		write(1, "\n", 1);
		(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
	}
	if (ft_is_option(cmd[1]) == TRUE)
		(*n) = 1;
}

void	ft_echo(char **cmd, t_gc *gc)
{
	int		i;
	int		k;
	int		n;
	int		f;
	char	buf[70000]; //changer ici
	i = 1;
	k = 0;
	n = 0;
	f = 0;
	no_arg(&n, cmd, gc);
	while (cmd[i] && ft_is_option(cmd[i]) == TRUE)
		i++;
	while (cmd[i])
	{
		if (f == 1)
			buf[k++] = ' ';
		ft_fill_buf(&i, &k, cmd, buf);
		f = 1;
	}
	if (n == 0)
		buf[k++] = '\n';
	if (write(1, buf, k) == -1)
		perror("minishell: echo: write error");
	(write_error(f, gc), gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
}
