/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/16 21:26:27 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_checkq(char *input)
{
	int		sq;
	int		dq;
	char	*ptr;

	sq = 0;
	dq = 0;
	ptr = input;
	while (*ptr)
	{
		if (*ptr == '\'' && !dq)
			sq = !sq;
		else if (*ptr == '\"' && !sq)
			dq = !dq;
		ptr++;
	}
	if (sq || dq)
		return (printf("minishell: unclosed quotes\n"));
	return (0);
}

static int	ft_unexptoken(char **result)
{
	int	i;

	if (!(*result))
		return (1);
	if (!ft_strcmp(result[0], "|"))
		return (printf("%s", UN_PIPE));
	i = 0;
	while (result[i])
	{
		if (!ft_strcmp(result[i], "|")
			&& result[i + 1] && !ft_strcmp(result[i + 1], "|"))
			return (printf("%s", UN_PIPE));
		if (ft_istok_(result[i]) && ft_strcmp(result[i], "|")
			&& result[i + 1] && ft_istok_(result[i + 1]))
		{
			printf("minishell: syntax error near unexpected token `");
			return (printf("%s'\n", result[i + 1]));
		}
		if (ft_istok_(result[i]) && !result[i + 1])
			return (printf("%s", UN_NLINE));
		i++;
	}
	return (0);
}

static char	**ft_fatalerror(char **array, t_gc *gc)
{
	char	**new_array;
	size_t	i;
	size_t	prev_size;

	if (!array[0])
		return (NULL);
	i = -1;
	prev_size = 0;
	while (array[++i])
		prev_size++;
	i = 0;
	new_array = gc_malloc((prev_size + 2) * sizeof(char *), gc);
	new_array[0] = ft_strdup_("\n", gc);
	while (i <= prev_size)
	{
		new_array[i + 1] = array[i];
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

static int	ft_mexitstatus(t_element *elements, t_gc *gc)
{
	elements->exit_status = ft_itoa(1, gc);
	gc_remove(gc, elements->arr);
	elements->arr = NULL;
	return (1);
}

void	ft_ft(t_element *elements, t_gc *gc)
{
	int		i;
	char	**tmp;

	i = -1;
	if (ft_checkq(elements->line) && ft_mexitstatus(elements, gc))
		return ;
	elements->arr = ft_split(ft_tokenize(elements->line,
				gc, elements), 0, 0, gc);
	while (elements->arr && elements->arr[++i])
	{
		if (ft_strcmp(elements->arr[i], "<<") == 0 && elements->arr[i + 1])
			i++;
		else
			elements->arr[i] = ft_concat(elements->arr[i], -1, 0, 0);
	}
	if (elements->arr && ft_unexptoken(elements->arr))
	{
		tmp = ft_fatalerror(elements->arr, gc);
		gc_remove(gc, elements->arr);
		elements->arr = NULL;
		elements->arr = tmp;
		if (elements->arr)
			elements->exit_status = ft_itoa(2, gc);
	}
}
