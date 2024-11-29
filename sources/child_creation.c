/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:06:24 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/29 16:44:40 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

void	other_cases(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	int	k;

	k = 0;
	while (k < elements->nb_cmd - 1)
	{
		if (cmd->fd_out == ERROR_OPEN || cmd->fd_in == ERROR_OPEN)
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		if (k == i)
		{
			if (cmd->fd_out == NO_TRY_OPEN)
			{
				if (dup2(elements->pipes[k][1], STDOUT_FILENO) == ERROR_OPEN)
				{
					perror("Error: dup2 out failed"); //changer ici a la fin
					(part_close(elements, k), gc_cleanup(gc), exit(EXIT_FAILURE));
				}
			}
			(close(elements->pipes[k][0]), close(elements->pipes[k][1]));
		}
		else if (k == i - 1)
		{
			if (cmd->fd_in == NO_TRY_OPEN)
			{
				if (dup2(elements->pipes[k][0], STDIN_FILENO) == -1)
				{
					perror("Error: dup2 in failed"); //changer ici a la fin
					(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
				}
			}
			close(elements->pipes[k][1]);
			close(elements->pipes[k][0]);
		}
		else
		{
			close(elements->pipes[k][1]);
			close(elements->pipes[k][0]);
		}
		k++;
	}
}

void	first_case(int i, t_element *elements, t_cmd *cmd, t_gc *gc) //norminette a travailler ici
{
	int	k;

	k = 0;
	if (cmd->fd_in == ERROR_OPEN || cmd->fd_out == ERROR_OPEN)
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	if (cmd->fd_in == NO_TRY_OPEN)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
		{
			perror("Error: dup2 in failed"); //changer ici a la fin
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	while (k < elements->nb_cmd - 1)
	{
		if (k == i)
		{
			if (cmd->fd_out == NO_TRY_OPEN)
			{
				if (dup2(elements->pipes[k][1], STDOUT_FILENO) == ERROR_OPEN)
				{
					perror("Error: dup2 out failed"); //changer ici a la fin
					(part_close(elements, k), gc_cleanup(gc), exit(EXIT_FAILURE));
				}
			}
			close(elements->pipes[k][0]);
			close(elements->pipes[k][1]);
		}
		else
		{
			close(elements->pipes[k][1]);
			close(elements->pipes[k][0]);
		}
		k++;
	}
}

void	last_case(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	int	k;

	k = 0;
	if (cmd->fd_out == ERROR_OPEN || cmd->fd_in == ERROR_OPEN)
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	if (cmd->fd_out == NO_TRY_OPEN) // possible de mettre les deux conditions a la suite
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("Error: dup2 out failed"); //changer ici a la fin
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	while (k < elements->nb_cmd - 1)
	{
		if (k == i - 1)
		{
			if (cmd->fd_in == NO_TRY_OPEN)
			{
				if (dup2(elements->pipes[k][0], STDIN_FILENO) == ERROR_OPEN)
					perror("Error: dup2 in failed"); //changer ici a la fin
				(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
			}
			close(elements->pipes[k][1]);
			close(elements->pipes[k][0]);
		}
		else
		{
			close(elements->pipes[k][1]);
			close(elements->pipes[k][0]);
		}
		k++;
	}
}

void	child_process(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	if (i == 0)
		first_case(i, elements, cmd, gc);
	else if (i == elements->nb_cmd - 1)
		last_case(i, elements, cmd, gc);
	else
		other_cases(i, elements, cmd, gc);
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
}

void	child_creation(t_element *elements, t_gc *gc) //prevoir la cas ou cmd[0]=NULL (mais on a des redir)
{
	int	i;

	i = 0;

	//while (i < ac - 3)
	elements->child_to_wait = elements->nb_cmd;
	while (i < elements->nb_cmd) //voir a partir de la
	{
		elements->pid_arr[i] = fork();
		if (elements->pid_arr[i] == -1)
		{
			elements->child_to_wait = elements->child_to_wait - 1; //a verifier si pas de pbm apres
			perror("Error: fork failed");
		}
		if (elements->pid_arr[i] == 0)
		{
			child_process(i, elements, elements->lst, gc);
			exec_command(elements, gc, i); //faire distinction entre buil in et autre ici
		}
		i++;
	}
}