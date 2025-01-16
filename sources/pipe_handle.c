/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:29:23 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/16 11:46:03 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

void	pipe_creation(t_element *elements, t_gc *gc)
{
	int	i;
	int	t;

	i = 0;
	//printf("ip = %d, nb cmd - 1 = %d\n", i, elements->nb_cmd - 1);
	while (i < elements->nb_cmd - 1)
	{
		//printf("pipe[%d] = %p\n", i, elements->pipes[i]);
		t = pipe(elements->pipes[i]);
		if (t == -1)
		{
			perror("Error: pipe creation failed");
			gc_cleanup(gc);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	close_pipes(t_element *elements)
{
	int	k;
	t_cmd *cmd;

	k = 0;
	while (k < elements->nb_cmd - 1)
	{
		close(elements->pipes[k][1]);
		close(elements->pipes[k][0]);
		k++;
	}
	cmd = elements->lst;
	while (cmd)
	{
		if (cmd->fd_in>0)
		{
			close(cmd->fd_in);
			cmd->fd_in = CLOSED;
		}
		if (cmd->fd_out>0)
		{
			close(cmd->fd_out);
			cmd->fd_out = CLOSED;
		}
		cmd = cmd->next;
	}
}

void	part_close(t_element *elements, int k)
{
	while (k < elements->nb_cmd - 1)
	{
		close(elements->pipes[k][1]);
		close(elements->pipes[k][0]);
		k++;
	}
}

void	wait_for_children(t_element *elements)
{
	int	i;

	i = 0;
	//perror("chelou");
	while (i < elements->child_to_wait)
	{
		//perror(" tres chelou");
		wait(NULL);
		i++;
	}
	//perror(" tres tres chelou");
}

void	free_std(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}
