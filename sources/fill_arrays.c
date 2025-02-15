/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_arrays.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:38:35 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/14 11:20:38 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static int	ft_lstsize(t_cmd **lst)
{
	int		s;
	t_cmd	*current;

	s = 0;
	current = *lst;
	if (*lst == NULL)
		return (0);
	while (current != NULL)
	{
		s++;
		current = current -> next;
	}
	return (s);
}

static void	ft_pipe_arr(t_element *elements, t_gc *gc)
{
	int	i;

	elements->pipes = gc_malloc((elements->nb_cmd) * sizeof(int *), gc);
	i = 0;
	while (i < elements->nb_cmd - 1)
	{
		elements->pipes[i] = gc_malloc(3 * sizeof(int), gc);
		i++;
	}
}

void	ft_fill_arrays(t_element *elements, t_gc *gc)
{
	elements->nb_cmd = ft_lstsize(&elements->lst);
	ft_pipe_arr(elements, gc);
	elements->pid_arr = gc_malloc(sizeof(int) * elements->nb_cmd, gc);
}
