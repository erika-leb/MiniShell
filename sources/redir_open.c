/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:14:15 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/05 14:54:51 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_handle_out(t_cmd *node, t_file *redir, t_element *elements, t_gc *gc) //gestion des entrees ??
{
	if ((node->fd_in == ERROR_OPEN || node->fd_out == ERROR_OPEN) && redir->token == HEREDOC) // il ya eu un redir invalide et c est un heredoc
	{
		node->active = FALSE;
		ft_open_heredoc_error(redir->name, elements, gc); // 0 si error 1 sinon
	}
	else if (node->fd_in != ERROR_OPEN && node->fd_out != ERROR_OPEN) // il n'y a pas eu de redir invalide pour l'instant
	{
		if (node->fd_in >= 0) //ft_close pour verifier qu'on a les droits pour fermer et close ensuite?
		{
			close(node->fd_in);
			node->fd_in = CLOSED;
			if(access(".here", F_OK) == 0)
				unlink(".here");
		}
		if (redir->token == HEREDOC)
		{
			//perror("bruce");
			node->fd_in = ft_open_heredoc(redir->name, elements, gc);
		}
		else
			ft_handle_no_here_out(node, redir, elements, gc);
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

void	ft_handle_in(t_cmd *node, t_file *redir, t_element *elements, t_gc *gc) //gestion des sorties ??
{
	(void) elements;
	(void) gc;

	if (node->fd_out != ERROR_OPEN && node->fd_in != ERROR_OPEN)
	{
		if (node->fd_out >= 0)
		{
			close(node->fd_out);
			node->fd_out = CLOSED;
		}
		if (redir->token == TRUNC)
		{
			// if (ft_strcmp(redir->name, ".here") == 0)
			// 	write(1, "forbidden name\n", 16); // faire peter l'enfant, faire pareil pour append
			// else
			node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		else
			node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (node->fd_out == ERROR_OPEN)
		{
			//perror("Error"); // iciiiiiiiiii
			ft_error_out(redir->name, elements, gc);
			//printf("fd__in = %d\n", node->fd_in);
			if (node->fd_in >= 0)
			{
				//perror("titou");
				close(node->fd_in);
				node->fd_in = CLOSED;
				if(access(".here", F_OK) == 0) //voir cas ou on modifie les droits pendant la lecture du heredoc
					unlink(".here");
			}
		}
	}
}

void	ft_fd_open(t_cmd *node, t_element *elements, t_gc *gc, int flag)
{
	t_file	*redir;

	//perror("sylvestre");
	//printf("flas = %d\n", flag);
	redir = node->redir;
	if (flag == 0)
	{
		//perror("alfred");
		while(redir && elements->line_valid == TRUE)
		{
			if (redir->token == TRUNC || redir->token == APPEND)
				ft_handle_in(node, redir, elements, gc);
			//perror("jack");
			if (redir->token == HEREDOC || redir->token == INPUT)
				ft_handle_out(node, redir, elements, gc);
			//perror("rose");
			redir = redir->next;
		}
	}
	else
	{
		//perror("batman");
		while(redir && elements->line_valid == TRUE)
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
	ft_strlcat(name_bis, tmp2, size); //ici
	ft_buff_error("minishell: ", elements, gc);
	ft_buff_error(name_bis, elements, gc);
	ft_buff_error(" ambiguous redirect\n", elements, gc);
	elements->exit_status = ft_itoa(1, gc); //verifier cela
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
		//perror("hockley");
		while (redir)
		{
			if (ft_strncmp("$\n", redir->name, 2) == 0)
				flag = 1;
			redir = redir->next;
		}
		//perror("caledon");
		ft_fd_open(current, elements, gc, flag);
		//perror("bastardo");
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

