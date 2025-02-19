/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_ebis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:13:53 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/16 21:41:53 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strjoin(char *s1, char *s2, t_gc *gc)
{
	char	*str;
	int		i;
	int		size1;
	int		size2;
	int		j;

	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	str = gc_malloc(sizeof(char) * (size1 + size2 + 1), gc);
	i = 0;
	j = 0;
	if (str == 0)
		return (NULL);
	while (i < size1)
	{
		str[i] = s1[i];
		i++;
	}
	while (i < size1 + size2)
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

void	write_all_err(char *s1, char *s2, t_element *elements, t_gc *gc)
{
	ft_buff_error("minishell: ", elements, gc);
	ft_buff_error(s1, elements, gc);
	ft_buff_error(s2, elements, gc);
	ft_write_error(elements, gc);
}

int	is_directory(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0)
	{
		return (S_ISDIR(sb.st_mode));
	}
	return (0);
}

void	exit_status(int status, t_element *elements, t_gc *gc)
{
	int	signal;

	if (WIFSIGNALED(status))
	{
		elements->exit_status = ft_itoa(WTERMSIG(status) + 128, gc);
		signal = WTERMSIG(status);
		if (signal == SIGQUIT)
			write(2, "Quit (core dumped)\n", 20);
		if (signal == SIGINT)
			write(2, "\n", 1);
	}
	else if (WIFEXITED(status))
		elements->exit_status = ft_itoa(WEXITSTATUS(status), gc);
}

int	ft_bst(char *name)
{
	int	i;

	i = -1;
	while (name[++i])
	{
		if (name[i] == '\t')
			return (1);
	}
	return (0);
}
