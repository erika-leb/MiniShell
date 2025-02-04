/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 14:44:29 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/04 14:51:26 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_newlines(const char *buffer, int bytes_read)
{
	int	i;
	int	lines;

	i = 0;
	lines = 0;
	while (i < bytes_read)
	{
		if (buffer[i] == '\n')
			lines++;
		i++;
	}
	return (lines);
}

static int	read_file(int fd)
{
	char	buffer[1024];
	int		bytes_read;
	int		lines;

	lines = 0;
	bytes_read = read(fd, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		lines += count_newlines(buffer, bytes_read);
		bytes_read = read(fd, buffer, sizeof(buffer));
	}
	if (bytes_read == -1)
	{
		perror("Erreur lors de la lecture du fichier");
		return (-1);
	}
	return (lines);
}

static int	count_lines(const char *filename)
{
	int		fd;
	int		lines;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Erreur lors de l'ouverture du fichier");
		return (-1);
	}
	lines = read_file(fd);
	close(fd);
	return (lines);
}

void	printf_mess_d(char *del, t_element *elements, t_gc *gc)
{
	int	nb_l;

	(void) elements;
	(void) gc;
	nb_l = count_lines(".here");
	write(2, "minishell: warning: here-document at line ", 43);
	ft_putstr_fd(ft_itoa(nb_l, gc), 2);
	write(2, " delimited by end-of-file (wanted `", 36);
	ft_putstr_fd(del, 2);
	write(2, "')\n", 4);
}
