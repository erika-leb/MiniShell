/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:36:34 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/16 21:31:29 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*here_exp(char *lign, t_element *elements, t_gc *gc, int flag)
{
	static char	buffer[SIZE_MAX];
	size_t		len;

	if (!lign)
		return (NULL);
	if (flag)
		return (lign);
	len = ft_strlen(lign);
	if (len >= sizeof(buffer))
		len = sizeof(buffer) - 1;
	ft_strncpy(buffer, lign, len);
	buffer[len] = '\0';
	return (ft_hereifexpand(buffer, elements, gc));
}

static int	ft_here(char *lign, char *dell, t_element *elements, t_gc *gc)
{
	if (g_signal != 0)
	{
		elements->exit_status = ft_itoa(128 + g_signal, gc);
		g_signal = 0;
		return (free(lign), 2);
	}
	if (lign == NULL)
	{
		(printf_mess_d(dell, elements, gc), free(lign));
		return (1);
	}
	if (ft_strcmp(lign, dell) == 0)
	{
		free(lign);
		return (1);
	}
	return (0);
}

int	ft_read_heredoc(char *del, int fd, t_element *elements, t_gc *gc)
{
	char	*lign;
	char	*dell;
	int		flag;
	int		k;

	flag = 0;
	dell = ft_concat(ft_strdup(del, gc), -1, 0, 0);
	if (ft_strcmp(dell, del))
		flag = 1;
	while (1)
	{
		lign = readline("> ");
		k = ft_here(lign, dell, elements, gc);
		if (k == 1)
			break ;
		if (k == 2)
			return (1);
		ft_putstr_fd(here_exp(lign, elements, gc, flag), fd);
		(ft_putstr_fd("\n", fd), free(lign));
	}
	return (0);
}

int	ft_open_heredoc_error(char *del, t_element *elements, t_gc *gc)
{
	int		fd;
	int		r;

	if (access(".here", F_OK) == 0)
		unlink(".here");
	fd = open(".here", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("Error opening .here"), -1);
	ft_handle_signal(3);
	r = ft_read_heredoc(del, fd, elements, gc);
	if (r == 1)
	{
		elements->line_valid = FALSE;
		close(fd);
		printf("line = %d\n", elements->line_valid);
		return (-1);
	}
	ft_handle_signal(0);
	close(fd);
	return (fd);
}

int	ft_open_heredoc(char *del, t_element *elements, t_gc *gc)
{
	int		fd;
	int		r;

	if (access(".here", F_OK) == 0)
		unlink(".here");
	fd = open(".here", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("Error opening .here"), -1);
	ft_handle_signal(3);
	r = ft_read_heredoc(del, fd, elements, gc);
	if (r == 1)
	{
		elements->line_valid = FALSE;
		close(fd);
		return (-1);
	}
	ft_handle_signal(0);
	close(fd);
	fd = open(".here", O_RDONLY);
	return (fd);
}
