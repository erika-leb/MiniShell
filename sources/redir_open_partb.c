/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open_partb.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:14:15 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/28 21:46:33 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_open_heredoc(char *del)
{
	int		fd;
	char	*lign; //a mettre dans

	if(access(".here", F_OK) == 0) // existe deja donc aura deja ete ferme avant normalement
		unlink(".here"); //peut on le supprimer si on a pas les droits ?
	fd = open(".here", O_WRONLY | O_CREAT , 0644);
	lign = readline("> ");
	if (lign == NULL)
		return (free(lign), (fd));
	while (ft_strcmp(lign, del) != 0)
	{
		ft_putstr_fd(lign, fd);
		ft_putstr_fd("\n", fd);
		free(lign);
		lign = readline("> ");
		if (lign == NULL)
		{
			free(lign);
			return (fd);
		}
	}
	free(lign);
	return (fd);
}

void	ft_handle_in(t_cmd *node, t_file *redir)
{
	if (node->fd_out != ERROR_OPEN && node->fd_in != ERROR_OPEN)
	{
		if (node->fd_out >= 0)
			close(node->fd_out);
		if (redir->token == TRUNC)
			node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (node->fd_out == ERROR_OPEN)
		{
			perror("Error");
			if (node->fd_in >= 0)
			{
				close(node->fd_in);
				if(access(".here", F_OK) == 0) //voir cas ou on modifie les droits pendant la lecture du heredoc
					unlink(".here");
			}
		}
	}
}

void	ft_handle_no_here_out(t_cmd *node, t_file *redir)
{
	node->fd_in = open(redir->name, O_RDONLY, 0644);
	if (node->fd_in == ERROR_OPEN)
	{
		perror("Error");
		if (node->fd_out >= 0)
		{
			close(node->fd_out);
			node->fd_out = -3;
		}
	}
}

void	ft_handle_out(t_cmd *node, t_file *redir)
{
	if ((node->fd_in == ERROR_OPEN || node->fd_out == ERROR_OPEN) && redir->token == HEREDOC)
		node->fd_in = ft_open_heredoc(redir->name);
	else if (node->fd_in != ERROR_OPEN && node->fd_out != ERROR_OPEN) // il n'y a pas eu de redir invalide pour l'instant
	{
		if (node->fd_in >= 0) //ft_close pour verifier qu'on a les droits pour fermer et close ensuite?
		{
			close(node->fd_in);
			if(access(".here", F_OK) == 0)
				unlink(".here");
		}
		if (redir->token == HEREDOC)
			node->fd_in = ft_open_heredoc(redir->name);
		else
			ft_handle_no_here_out(node, redir);
		// {
		// 	node->fd_in = open(redir->name, O_RDONLY, 0644);
		// 	if (node->fd_in == ERROR_OPEN)
		// 	{
		// 		perror("Error");
		// 		if (node->fd_out >= 0)
		// 		{
		// 			close(node->fd_out);
		// 			node->fd_out = -3;
		// 		}
		// 	}
		// }
	}
}