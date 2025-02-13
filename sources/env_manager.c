/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/13 15:06:05 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static t_env *ft_filltoa(char **array, t_env *current, size_t i, t_gc *gc)
{
	size_t  j;
	size_t  len;

	if (current->key)
		len = ft_strlen(current->name) + ft_strlen(current->key) + 2;
	else
		len = ft_strlen(current->name) + 2;
	array[i] = gc_malloc(len * sizeof(char), gc);
	j = -1;
	while (current->name[++j])
		array[i][j] = current->name[j];
	array[i][j] = '\0';
	if (current->key)
		ft_strcat(ft_strcat(array[i], "="), current->key);
	return (current->next);
}

char **ft_ltoa(t_env *head, t_gc *gc)
{
	size_t  count;
	size_t  i;
	t_env   *current;
	char    **array;

	if (!head)
		return NULL;
	current = head;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = gc_calloc(count + 1, sizeof(char *), gc);
	current = head;
	i = 0;
	while (i < count)
		current = ft_filltoa(array, current, i++, gc);
	return (array);
}

static char	*ft_strchr(char const *str, int c)
{
	unsigned int	i;
	char			char_c;

	i = 0;
	char_c = (char)c;
	while (str[i] != '\0')
	{
		if (str[i] == char_c)
			return ((char *)&str[i]);
		i++;
	}
	if (str[i] == char_c)
		return ((char *)&str[i]);
	return (NULL);
}

void ft_env(char **array, char **cmds, t_gc *gc)
{
	int		i;
	int		size;
	char	*res;

	if (cmds && cmds[1])
	{
		write(2,"minshell: env: No option(s) or argument(s) allowed\n", 52);
		if (cmds[1][0] == '-')
			(gc_cleanup(gc), free_std(), exit(125));
		(gc_cleanup(gc), free_std(), exit(127));
	}
	i = 0;
	while (array[i])
	{
		if (ft_strchr(array[i],'='))
		{
			res = ft_strjoin_(array[i], "\n", gc);
			size = ft_strlen(res);
			write(1, res, size);
		}
		i++;
	}
	(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
}
