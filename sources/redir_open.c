/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:14:15 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/14 18:58:51 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_handle_out(t_cmd *node, t_file *redir, t_element *elm, t_gc *gc)
{
	if ((node->fd_in == ERROR_OPEN || node->fd_out == ERROR_OPEN)
		&& redir->token == HEREDOC) //entrees
	{
		node->active = FALSE;
		ft_open_heredoc_error(redir->name, elm, gc);
	}
	else if (node->fd_in != ERROR_OPEN && node->fd_out != ERROR_OPEN)
	{
		if (node->fd_in >= 0)
		{
			close(node->fd_in);
			node->fd_in = CLOSED;
			if (access(".here", F_OK) == 0)
				unlink(".here");
		}
		if (redir->token == HEREDOC)
			node->fd_in = ft_open_heredoc(redir->name, elm, gc);
		else
			no_here_out(node, redir, elm, gc);
	}
}

void	ft_handle_in(t_cmd *node, t_file *redir, t_element *elm, t_gc *gc) //gestion des sorties ??
{
	(void) elm;
	(void) gc;
	if (node->fd_out != ERROR_OPEN && node->fd_in != ERROR_OPEN)
	{
		if (node->fd_out >= 0)
		{
			close(node->fd_out);
			node->fd_out = CLOSED;
		}
		if (redir->token == TRUNC)
			node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (node->fd_out == ERROR_OPEN)
		{
			ft_error_out(redir->name, elm, gc);
			if (node->fd_in >= 0)
			{
				close(node->fd_in);
				node->fd_in = CLOSED;
				if (access(".here", F_OK) == 0)
					unlink(".here");
			}
		}
	}
}

void	ft_fd_open(t_cmd *node, t_element *elements, t_gc *gc, int flag)
{
	t_file	*redir;

	redir = node->redir;
	if (flag == 0)
	{
		while (redir && elements->line_valid == TRUE)
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
		while (redir && elements->line_valid == TRUE)
		{
			if (redir->token == HEREDOC)
				ft_handle_out(node, redir, elements, gc);
			redir = redir->next;
		}
	}
}

void	charge_ambig(t_element *elements, char *name, t_gc *gc) // corriger ici demain avec ls | ls < $a | cat
{
	char	*tmp2;
	char	*name_bis;
	size_t	size;

	size = ft_strlen(name);
	tmp2 = ft_substr(name, 2, size - 2, gc);
	name_bis = gc_malloc((size) * sizeof(char), gc);
	name_bis[0] = '$';
	name_bis[1] = '\0';
	ft_strlcat(name_bis, tmp2, size);
	ft_buff_error("minishell: ", elements, gc);
	ft_buff_error(name_bis, elements, gc);
	ft_buff_error(" ambiguous redirect\n", elements, gc);
	elements->exit_status = ft_itoa(1, gc);
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
