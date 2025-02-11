/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_hedge_cases.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:49:44 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/10 19:49:41 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_child_events(t_element *elements, t_gc *gc, t_cmd *current)
{
	if (elements->nb_cmd == 1 && !elements->lst->cmd[0])
	{
		elements->child_to_wait = 0;
		return (1);
	}
	if (elements->lst->cmd[0] && ft_strcmp(elements->lst->cmd[0], "\n") == 0)
	{
		if (current->fd_in >= 0)
		{
			close(current->fd_in);
			current->fd_in = CLOSED;
		}
		if (current->fd_out >= 0)
		{
			close(current->fd_out);
			current->fd_out = CLOSED;
		}
		return (1);
	}
	if (elements->nb_cmd == 1 && (ft_strcmp(elements->lst->cmd[0], "exit") == 0
			|| ft_strcmp(elements->lst->cmd[0], "export") == 0
			|| ft_strcmp(elements->lst->cmd[0], "unset") == 0
			|| ft_strcmp(elements->lst->cmd[0], "cd") == 0))
		return (built_in_no_child(elements, gc), 1);
	return (0);
}

void	hedge_child_cases(t_element *elements, t_gc *gc, t_cmd	*current)
{
	t_file	*redir;

	if (!current->cmd[0])
	{
		close_pipes(elements);
		(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
	}
	redir = current->redir;
	while (redir)
	{
		if (ft_strncmp("$\n", redir->name, 2) == 0)
		{
			close_pipes(elements);
			(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
		}
		redir = redir->next;
	}
}
