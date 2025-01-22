/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open_parta.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:14:15 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/22 15:15:29 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_fd_open(t_cmd *node, t_element *elements, t_gc *gc, int flag)
{
	t_file	*redir;

	redir = node->redir;
	if (flag == 0)
	{
		while(redir)
		{
			if (redir->token == TRUNC || redir->token == APPEND)
				ft_handle_in(node, redir, elements, gc);
			if (redir->token == HEREDOC || redir->token == INPUT)
				ft_handle_out(node, redir, elements, gc);
			redir = redir->next;
		}
	}
	else
	{
		while(redir)
		{
			if (redir->token == HEREDOC)
				ft_handle_out(node, redir, elements, gc);
			redir = redir->next;
		}
	}
}

void	charge_ambig(t_element *elements, char *name, t_gc *gc)
{
	ft_buff_error("minishell: ", elements, gc);
	ft_buff_error(name, elements, gc);
	ft_buff_error(" ambiguous redirect\n", elements, gc);
}

void	handle_redir(t_cmd **lst, t_element *elements, t_gc *gc)
{
	t_cmd	*current;
	t_file	*redir;
	int		flag;

	current = *lst;
	while (current)
	{
		flag = 0;
		redir = current->redir;
		while (redir)
		{
			if (ft_strncmp("$\n", redir->name, 2) == 0)
				flag = 1;
			redir = redir->next;
		}
		ft_fd_open(current, elements, gc, flag);
		redir = current->redir;
		while (redir)
		{
			if (ft_strncmp("$\n", redir->name, 2) == 0)
				charge_ambig(elements, redir->name, gc);
			redir = redir->next;
		}
		current = current->next;
	}
}

