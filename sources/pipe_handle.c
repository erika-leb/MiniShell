/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:29:23 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/16 19:01:33 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_creation(t_element *elements, t_gc *gc)
{
	int	i;
	int	t;

	i = 0;
	while (i < elements->nb_cmd - 1)
	{
		t = pipe(elements->pipes[i]);
		if (t == -1)
		{
			perror("Epipe creation failed");
			gc_cleanup(gc);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	close_pipes(t_element *elements)
{
	int		k;
	t_cmd	*cmd;

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
		if (cmd->fd_in > 0)
		{
			close(cmd->fd_in);
			cmd->fd_in = CLOSED;
		}
		if (cmd->fd_out > 0)
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

void	wait_for_children(t_element *elements, t_gc *gc)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	//dprintf(2, "child to wait = %d\n", elements->child_to_wait);
	if (elements->child_to_wait == 0)
		return ;
	while (i < elements->child_to_wait)
	{
		//dprintf(2, "waiting for %d\n", elements->pid_arr[i]);
		waitpid(elements->pid_arr[i], &status, 0);
		i++;
	}
	exit_status(status, elements, gc);
}

void	free_std(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}
