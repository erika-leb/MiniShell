/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 13:13:37 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/26 17:39:06 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_free(char *s)
{
	free(s);
	s = NULL;
	return (s);
}

char	*clean_stash_buffer(char *stash, char *buffer, int *n, t_gc *gc)
{
	int		i;
	int		j;
	char	*temp;

	i = ft_strchr(buffer, '\n');
	j = ft_strchr(stash, '\n');
	if (j != -1)
	{
		temp = ft_substr(stash, j + 1, ft_strlen(stash) - j - 1);
		free(stash);
		stash = ft_strdup(temp, gc);
		return (free(temp), free(buffer), stash);
	}
	else
	{
		if (stash != NULL)
			free(stash);
		stash = NULL;
		if (i != -1)
			stash = ft_substr(buffer, i + 1, *n - i - 1);
		return (free(buffer), stash);
	}
}

char	*read_and_stock(int fd, char *line, char *buffer, int *n)
{
	int			i;
	int			f;
	char		*temp;
	char		*tmp;

	i = -1;
	while (i == -1 && *n == BUFFER_SIZE && ft_strchr(line, '\n') == -1)
	{
		*n = read(fd, buffer, BUFFER_SIZE);
		if (*n == -1)
			return (NULL);
		i = ft_strchr(buffer, '\n');
		f = (*n);
		if (*n != 0)
		{
			if (i != -1)
				f = i + 1;
			tmp = ft_substr(buffer, 0, f);
			temp = ft_strdup(line, gc);
			free(line);
			line = ft_strjoin(temp, tmp);
			(free(temp), free(tmp));
		}
	}
	return (line);
}

char	*get_next_line(int fd, t_gc *gc)
{
	char		*buffer;
	static char	*stash[10240];
	char		*line;
	int			n;

	if (fd == -1 || BUFFER_SIZE < 0 || read(fd, stash[fd], 0) < 0)
	{
		if (stash[fd] != NULL)
			gc_remove(stash[fd], gc);
		return (NULL);
	}
	n = BUFFER_SIZE;
	buffer = gc_calloc(BUFFER_SIZE + 1, sizeof(char), gc);
	if (buffer == NULL)
		return (NULL);
	line = NULL;
	if (stash[fd] != NULL && stash[fd][0] != '\0' &&
		ft_strchr(stash[fd], '\n') == -1)
		line = ft_strdup(stash[fd], gc);
	else if (stash[fd] != NULL && stash[fd][0] != '\0' &&
		ft_strchr(stash[fd], '\n') != -1)
		line = ft_substr(stash[fd], 0, ft_strchr(stash[fd], '\n') + 1);
	line = read_and_stock(fd, line, buffer, &n);
	stash[fd] = clean_stash_buffer(stash[fd], buffer, &n, gc);
	return (line);
}
