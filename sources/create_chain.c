/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_chain.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:56:56 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/13 18:28:05 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ft_is_redir(char *s)
{
	if (ft_strcmp(s, "<") == 0 || ft_strcmp(s, "<<") == 0
		|| ft_strcmp(s, ">") == 0 || ft_strcmp(s, ">>") == 0)
		return (true);
	return (false);
}

bool	ft_is_str(char *s)
{
	if (ft_strcmp(s, "\n|") == 0)
		return (true);
	if (ft_strcmp(s, "\n<") == 0)
		return (true);
	if (ft_strcmp(s, "\n>") == 0)
		return (true);
	if (ft_strcmp(s, "\n<<") == 0)
		return (true);
	if (ft_strcmp(s, "\n>>") == 0)
		return (true);
	return (false);
}

char	**cmd_arr(t_arg *arg, t_element *elements, t_gc *gc)
{
	int		s_arr;
	char	**arr;

	s_arr = nb_arg(arg, elements, gc);
	arr = gc_malloc(sizeof(char *) * (s_arr + 1), gc);
	ft_fill_arr(arr, arg, elements, gc);
	return (arr);
}

void	create_chain(t_arg *arg, t_element *elements, t_gc *gc)
{
	t_cmd	*new;
	t_cmd	*current;

	new = gc_malloc(sizeof(t_cmd), gc);
	new->cmd = NULL;
	new->redir = create_redir(arg, elements, gc);
	new->cmd = cmd_arr(arg, elements, gc);
	new->fd_in = -2;
	new->fd_out = -2;
	new->active = TRUE;
	new->next = NULL;
	if (!*(arg->lst))
	{
		*(arg->lst) = new;
		return ;
	}
	current = *(arg->lst);
	while (current -> next != NULL)
		current = current -> next;
	current -> next = new;
}
