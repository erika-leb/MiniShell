/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:16:26 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/14 14:22:56 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error_out(char *name, t_element *elements, t_gc *gc)
{
	if (errno == 2)
	{
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(name, elements, gc);
		ft_buff_error(": No such file or directory\n", elements, gc);
	}
	else if (errno == 13)
	{
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(name, elements, gc);
		ft_buff_error(": Permission denied\n", elements, gc);
	}
	else
	{
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(name, elements, gc);
		ft_buff_error(": ", elements, gc);
		ft_buff_error(strerror(errno), elements, gc);
		ft_buff_error("\n", elements, gc);
	}
	elements->exit_status = ft_itoa(1, gc);
}

void	no_here_out(t_cmd *node, t_file *redir, t_element *elements, t_gc *gc)
{
	node->fd_in = open(redir->name, O_RDONLY, 0644);
	if (node->fd_in == ERROR_OPEN)
	{
		if (node->fd_out >= 0)
		{
			close(node->fd_out);
			node->fd_out = -3;
		}
		ft_error_out(redir->name, elements, gc);
	}
}
