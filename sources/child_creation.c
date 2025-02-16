/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:06:24 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/16 17:15:10 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	uniq_case(t_element *elements, t_cmd *cmd, t_gc *gc)
{
	if (cmd->fd_in == ERROR_OPEN || cmd->fd_in == ERROR_OPEN)
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	if (cmd->fd_in >= 0)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
		{
			perror("dup2 failed");
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	if (cmd->fd_out >= 0)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("dup2 failed");
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
}

void	child_process(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	if (cmd->fd_out == ERROR_OPEN || cmd->fd_in == ERROR_OPEN)
	{
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	if (i == 0 && elements->nb_cmd == 1)
		uniq_case(elements, cmd, gc);
	else
	{
		all_cases(i, elements, cmd, gc);
	}
	if (cmd->fd_in >= 0)
	{
		close(cmd->fd_in);
		cmd->fd_in = CLOSED;
	}
	if (cmd->fd_out >= 0)
	{
		close(cmd->fd_out);
		cmd->fd_out = CLOSED;
	}
	close_other_redir(i, elements);
}

void	ft_handle_child(t_element *elements, t_gc *gc, int i, t_cmd *current)
{
	// int j;

	elements->pid_arr[i] = fork();
	if (elements->pid_arr[i] == -1)
	{
		elements->child_to_wait = elements->child_to_wait - 1;
		perror("fork failed");
	}
	if (elements->pid_arr[i] == 0)
	{
		//dprintf(2, "creation process id = %d\n", getpid());
		hedge_child_cases(elements, gc, current);
		child_process(i, elements, current, gc);
		// printf("\n APRES FORK \n\n");
		// print_cmd_list(elements->lst);
		if (is_built_in(current->cmd[0]) == TRUE)
			ft_built_in(elements, current->cmd, gc);
		else
			exec_command(elements, gc, i);
		// if (g_signal == 13)
		// {
		// 	perror("ici");
		// 	g_signal = 0;
		// 	gc_cleanup(gc);
		// 	free_std();
		// 	// j = 3;
		// 	// while (j < 253)
		// 	// {
		// 	// 	close(j);
		// 	// 	j++;
		// 	// }
		// 	exit(128 + 13);
		// }
	}
}

void	child_creation(t_element *elements, t_gc *gc)
{
	int		i;
	t_cmd	*current;

	i = 0;
	current = elements->lst;
	elements->child_to_wait = elements->nb_cmd;
	// printf("\n AVANT FORK \n\n");
	// print_cmd_list(elements->lst);
	if (no_child_events(elements, gc, current) == 1)
	{
		elements->child_to_wait = 0;
		return ;
	}
	ft_handle_signal(1);
	while (i < elements->nb_cmd)
	{
		if (current->active == TRUE)
			ft_handle_child(elements, gc, i, current);
		else
			elements->child_to_wait = elements->child_to_wait - 1;
		i++;
		current = current->next;
	}
}
