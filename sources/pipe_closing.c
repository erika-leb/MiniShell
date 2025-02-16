/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_closing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:58:06 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/16 19:28:29 by ele-borg         ###   ########.fr       */
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
	if (cmd->fd_in > 2) // Évite de fermer stdin (0), stdout (1), stderr (2) a garder ?
	{
		close(cmd->fd_in);
		cmd->fd_in = CLOSED;
	}
}

void	last_cmd_with_valid_outfile(t_element *elm, t_cmd *cmd, t_gc *gc)
{
	//perror("on dup b ici");
	if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
	{
		perror("dup2 failed");
		(close_pipes(elm), gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	if (cmd->fd_out > 2) // Évite de fermer stdin (0), stdout (1), stderr (2) a gar
	{
		close(cmd->fd_out);
		cmd->fd_out = CLOSED;
	}
}

void	dup_close_read_pipe(int k, t_element *elm, t_cmd *cmd, t_gc *gc)
{
	//perror("wtf");
	if (cmd->fd_out == NO_TRY_OPEN)
	{
		//perror("puta");
		//dprintf(2, "process id = %d\n", getpid());
		//dprintf(2, "process id = %d\n", getpid());
		if (dup2(elm->pipes[k][1], STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("dup2 failed");
			(part_close(elm, k), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	//printf("Process %d: After dup2, fd_out = %d\n", getpid(), elm->pipes[k][1]);
	if (cmd->fd_out > 0)
	{
		//perror("ouh la al");
		if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("dup2 failed");
			(part_close(elm, k), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	//dprintf(2, "process id = %d close pipe[%d][1] et [%d][0]\n", getpid(), k, k);
	(close(elm->pipes[k][0]), close(elm->pipes[k][1]));
}

void	dup_close_write_pipe(int k, t_element *elm, t_cmd *cmd, t_gc *gc)
{
	if (cmd->fd_in == NO_TRY_OPEN)
	{
		//perror("pour cmd 0");
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
	//dprintf(2, "process id = %d close pipe[%d][1] et [%d][0]\n", getpid(), k, k);
	(close(elm->pipes[k][1]), close(elm->pipes[k][0]));
}

void	all_cases(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	int		k;

	k = 0;
	//perror("mora");
	if (i == 0 && cmd->fd_in >= 0)
		first_cmd_with_valid_infile(elements, cmd, gc);
	if (i == elements->nb_cmd - 1 && cmd->fd_out >= 0)
		last_cmd_with_valid_outfile(elements, cmd, gc);
	while (k < elements->nb_cmd - 1)
	{
		//perror("aloho");
		if (k == i)
			dup_close_read_pipe(k, elements, cmd, gc);
		else if (k == i - 1)
			dup_close_write_pipe(k, elements, cmd, gc);
		else
		{
			//dprintf(2, "process id = %d close pipe[%d][1] et [%d][0]\n", getpid(), k, k);
			close(elements->pipes[k][1]);
			close(elements->pipes[k][0]);
		}
		// close(elements->pipes[k][1]);
		// close(elements->pipes[k][0]);
		k++;
	}
	//printf("Process %d: cmd->fd_in = %d, cmd->fd_out = %d\n", getpid(), cmd->fd_in, cmd->fd_out);

}
