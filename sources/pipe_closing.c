/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_closing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:58:06 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/13 17:31:19 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

void	first_cmd_with_valid_infile(t_element *elements, t_cmd *cmd, t_gc *gc)
{
	//perror("lemoncito");
	if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
	{
		perror("Error: dup2 in failed"); //changer ici a la fin
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	//(close(elements->pipes[0][0]), close(elements->pipes[0][1]));
}

void	last_cmd_with_valid_outfile(t_element *elements, t_cmd *cmd, t_gc *gc)
{
	perror("dara");
	if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
	{
		perror("Error: dup2 out failed"); //changer ici a la fin
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	//(close(elements->pipes[elements->nb_cmd - 1][0]), close(elements->pipes[elements->nb_cmd - 1][1]));
}

void	dup_and_close_read_pipe(int k, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	if (cmd->fd_out == NO_TRY_OPEN)
	{
		if (dup2(elements->pipes[k][1], STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("Error: dup2 out failed"); //changer ici a la fin
			(part_close(elements, k), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	if (cmd->fd_out > 0)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("Error: dup2 out failed"); //changer ici a la fin
			(part_close(elements, k), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	(close(elements->pipes[k][0]), close(elements->pipes[k][1]));
}

void	dup_and_close_write_pipe(int k, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	if (cmd->fd_in == NO_TRY_OPEN)
	{
		//perror("test");
		//check_fds("ici");
		    // Tentative d'écriture dans un pipe fermé



		//////////////////////////////////
		// dprintf(2, "ENCORE APRES\n");
		// dprintf(2, "k = %d\n", k);
		// if (fcntl(elements->pipes[k][0], F_GETFD) == -1) {
		// 	perror("pipefd[] is invalid");
		// }
		// else
		// 	perror("pipefd[0] is valid");
		// if (fcntl(elements->pipes[k][1], F_GETFD) == -1)
		// 	perror("pipefd[1] is invalid");
		// else
		// 	perror("pipefd[1] is valid");
		//////////////////////////////////
		if (dup2(elements->pipes[k][0], STDIN_FILENO) == -1)
		{
			perror("Error: dup2 in failed"); //changer ici a la fin
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	if (cmd->fd_in > 0)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
		{
			perror("Error: dup2 in failed"); //changer ici a la fin
			(part_close(elements, k), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	(close(elements->pipes[k][1]), close(elements->pipes[k][0]));
}

void	all_cases(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	int		k;

	k = 0;
	//perror("malamia");
	//dprintf(2,"i = %d\n", i);
		// dprintf(2, "AVANT\n");
		// if (fcntl(elements->pipes[k][0], F_GETFD) == -1) {
		// 	dprintf(2, "pipefd[%d][0] is invalid, i = %d\n", k, i);
		// }
		// else
		// 	dprintf(2, "pipefd[%d][0] is valid, i = %d\n", k, i);
		// if (fcntl(elements->pipes[k][1], F_GETFD) == -1)
		// 	dprintf(2, "pipefd[%d][1] is invalid,i = %d\n", k, i);
		// else
		// 	dprintf(2, "pipefd[%d][1] is invalid, i = %d\n", k, i);
		// // dprintf(2, "APRES\n");
	if (i == 0 && cmd->fd_in >= 0) // cas premier
	{
		//perror("jueputa");
		first_cmd_with_valid_infile(elements, cmd, gc);
	}
	// {
	// 	if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
	// 	{
	// 		perror("Error: dup2 in failed"); //changer ici a la fin
	// 		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	// 	}
	// }
	if (i == elements->nb_cmd - 1 && cmd->fd_out >= 0) // cas dernier
	{
		//perror("que rico");
		last_cmd_with_valid_outfile(elements, cmd, gc);
	}
	// {
	// 	if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
	// 	{
	// 		perror("Error: dup2 out failed"); //changer ici a la fin
	// 		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	// 	}
	// }
	// printf("\n CAS 3 \n\n");
	// print_cmd_list(elements->lst);
	while (k < elements->nb_cmd - 1)
	{
		if (k == i)   // out
		{
			//perror("si quieres que te singue");
			dup_and_close_read_pipe(k, elements, cmd, gc);
			// if (cmd->fd_out == NO_TRY_OPEN)
			// {
			// 	if (dup2(elements->pipes[k][1], STDOUT_FILENO) == ERROR_OPEN)
			// 	{
			// 		perror("Error: dup2 out failed"); //changer ici a la fin
			// 		(part_close(elements, k), gc_cleanup(gc), exit(EXIT_FAILURE));
			// 	}
			// }
			// if (cmd->fd_out > 0)
			// {
			// 	if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
			// 	{
			// 		perror("Error: dup2 out failed"); //changer ici a la fin
			// 		(part_close(elements, k), gc_cleanup(gc), exit(EXIT_FAILURE));
			// 	}
			// }
			// (close(elements->pipes[k][0]), close(elements->pipes[k][1]));
		}
		else if (k == i - 1)  // in
		{
			//perror("mirame mal");
			dup_and_close_write_pipe(k, elements, cmd, gc);
			// if (cmd->fd_in == NO_TRY_OPEN)
			// {
			// 	if (dup2(elements->pipes[k][0], STDIN_FILENO) == -1)
			// 	{
			// 		perror("Error: dup2 in failed"); //changer ici a la fin
			// 		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
			// 	}
			// }
			// if (cmd->fd_in > 0)
			// {
			// 	if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
			// 	{
			// 		perror("Error: dup2 in failed"); //changer ici a la fin
			// 		(part_close(elements, k), gc_cleanup(gc), exit(EXIT_FAILURE));
			// 	}
			// }
			// (close(elements->pipes[k][1]), close(elements->pipes[k][0]));
		}
		else
		{
			//perror("despues de mi");
			(close(elements->pipes[k][1]), close(elements->pipes[k][0]));
		}
		k++;
	}
}
