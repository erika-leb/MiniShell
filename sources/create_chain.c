/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_chain.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:56:56 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/12 14:58:14 by aisidore         ###   ########.fr       */
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

void	create_chain(t_arg *arg, int i, int last_i, t_element *elements, t_gc *gc)
{
	t_cmd	*new;
	t_cmd	*current;

	new = gc_malloc(sizeof(t_cmd), gc);
	new->cmd = NULL;
	new->redir = create_redir(arg->tab, i, last_i, elements, gc);
	new->cmd = cmd_arr(arg->tab, i, last_i, elements, gc);
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
