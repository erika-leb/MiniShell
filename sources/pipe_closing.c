/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_closing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:58:06 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/14 18:57:18 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	first_cmd_with_valid_infile(t_element *elm, t_cmd *cmd, t_gc *gc)
{
	if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
	{
		perror("dup2 failed");
		(close_pipes(elm), gc_cleanup(gc), exit(EXIT_FAILURE));
	}
}

void	last_cmd_with_valid_outfile(t_element *elm, t_cmd *cmd, t_gc *gc)
{
	if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
	{
		perror("dup2 failed");
		(close_pipes(elm), gc_cleanup(gc), exit(EXIT_FAILURE));
	}
}

void	dup_close_read_pipe(int k, t_element *elm, t_cmd *cmd, t_gc *gc)
{
	if (cmd->fd_out == NO_TRY_OPEN)
	{
		if (dup2(elm->pipes[k][1], STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("dup2 failed");
			(part_close(elm, k), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	if (cmd->fd_out > 0)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("dup2 failed");
			(part_close(elm, k), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	(close(elm->pipes[k][0]), close(elm->pipes[k][1]));
}

void	dup_close_write_pipe(int k, t_element *elm, t_cmd *cmd, t_gc *gc)
{
	if (cmd->fd_in == NO_TRY_OPEN)
	{
		if (dup2(elm->pipes[k][0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			(close_pipes(elm), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	if (cmd->fd_in > 0)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
		{
			perror("dup2 failed");
			(part_close(elm, k), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	(close(elm->pipes[k][1]), close(elm->pipes[k][0]));
}

void	all_cases(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	int		k;

	k = 0;
	if (i == 0 && cmd->fd_in >= 0)
		first_cmd_with_valid_infile(elements, cmd, gc);
	if (i == elements->nb_cmd - 1 && cmd->fd_out >= 0)
		last_cmd_with_valid_outfile(elements, cmd, gc);
	while (k < elements->nb_cmd - 1)
	{
		if (k == i)
			dup_close_read_pipe(k, elements, cmd, gc);
		else if (k == i - 1)
			dup_close_write_pipe(k, elements, cmd, gc);
		else
			(close(elements->pipes[k][1]), close(elements->pipes[k][0]));
		k++;
	}
}
