/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:29:23 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/28 10:59:54 by ele-borg         ###   ########.fr       */
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
	//perror("tom");
	while (k < elements->nb_cmd - 1)
	{
		close(elements->pipes[k][1]);
		close(elements->pipes[k][0]);
		k++;
	}
	//perror("jerry");
	cmd = elements->lst;
	while (cmd)
	{
		//perror("mimi");
		//printf("fd-in avant = %d\n", cmd->fd_in);
		if (cmd->fd_in>0)
		{
			close(cmd->fd_in);
			cmd->fd_in = CLOSED;
		}
		//printf("fd-in apres = %d\n", cmd->fd_in);
		//perror("siku");
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

void	exit_status(int	status, t_element *elements, t_gc *gc)
{
	int	signal;

	//perror("jake");
	if (WIFSIGNALED(status)) //verification si process terminee a cause d un signal
	{
		//perror("peralta");
		elements->exit_status = ft_itoa(WTERMSIG(status) + 128, gc);
		signal = WTERMSIG(status);
		if (signal == SIGQUIT)
			write(2, "Quit (core dumped)\n", 20);
		if (signal == SIGINT)
			write(2, "\n", 1);
	}
	else if (WIFEXITED(status))  //verification si process terminee normalement
	{
		//perror("holt");
		elements->exit_status = ft_itoa(WEXITSTATUS(status), gc);
		//printf("status = %s\n", elements->exit_status);
	}
}

void	wait_for_children(t_element *elements, t_gc *gc)
{
	int	i;
	int	status;

	i = 0;
	status =0;
	//perror("chelou");
	if (elements->child_to_wait == 0)
	{
		elements->exit_status = ft_itoa(errno, gc);
		return ;
	}
	while (i < elements->child_to_wait)
	{
		//perror(" tres chelou");
		waitpid(elements->pid_arr[i], &status, 0);
		//wait(NULL);
		i++;
	}
	exit_status(status, elements, gc);
	//perror(" tres tres chelou");
}

void	free_std(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}
