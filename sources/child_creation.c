/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:06:24 by ele-borg          #+#    #+#             */
/*   Updated: 2024/12/01 23:56:59 by ele-borg         ###   ########.fr       */
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
		else if (k == i)
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
	dprintf(2, "first fdout =%d fd_in = %d\n", cmd->fd_out, cmd->fd_in);
	if (cmd->fd_in == ERROR_OPEN || cmd->fd_out == ERROR_OPEN)
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	//printf("fd_in = %d\n", cmd->fd_in);
	if (cmd->fd_in >= 0)
	{
		//cmd->fd_in = STDIN_FILENO;
		// perror("test");
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
			//dprintf(2, "calcul nbk k, k = %d\n", k);
			//perror("on est la");
			if (cmd->fd_out == NO_TRY_OPEN)
			{
				//perror("passage par la");
				if (dup2(elements->pipes[k][1], STDOUT_FILENO) == ERROR_OPEN)
				{
					perror("Error: dup2 out failed"); //changer ici a la fin
					(part_close(elements, k), gc_cleanup(gc), exit(EXIT_FAILURE));
				}
			// perror("bizarre");
			// dprintf(2, "premiere cmd fdin = %d fd-out = %d\n", cmd->fd_in, cmd-> fd_out);
			// perror("bizarre2");
			//dprintf(2, "1Avant fermeture, pipe[%d]: read = %d, write = %d\n", k, elements->pipes[k][0], elements->pipes[k][1]);
			//dprintf(2,"k = %d\n", k);
				close(elements->pipes[k][0]);
				close(elements->pipes[k][1]);
				//dprintf(2, "1Après fermeture, pipe[%d]: read = %d, write = %d\n", k, elements->pipes[k][0], elements->pipes[k][1]);
			}
		}
		else
		{
			//perror("passage ici2");
			//dprintf(2, "k = %d\n", k);
			//dprintf(2, "2Avant fermeture, pipe[%d]: read = %d, write = %d\n", k, elements->pipes[k][0], elements->pipes[k][1]);
			close(elements->pipes[k][0]);
			close(elements->pipes[k][1]);
			//dprintf(2, "2Après fermeture, pipe[%d]: read = %d, write = %d\n", k, elements->pipes[k][0], elements->pipes[k][1]);
			//perror("passage la");
		}
		k++;
		//perror("bizarre4");
		//dprintf(2, "k = %d, i = %d, elements->nb_cmd = %d\n", k, i, elements->nb_cmd);
	}
	//perror("qutre test");
	//dprintf(2, "premiere cmd fdin = %d fd-out = %d\n", cmd->fd_in, cmd-> fd_out);
	
}

void	last_case(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	int	k;

	k = 0;
	perror("on passe ici");
	dprintf(2, "last fdout =%d fd_in = %d\n", cmd->fd_out, cmd->fd_in);
	if (cmd->fd_out == ERROR_OPEN || cmd->fd_in == ERROR_OPEN)
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	if (cmd->fd_out >= 0) // possible de mettre les deux conditions a la suite
	{
		//cmd->fd_out = STDOUT_FILENO;
		perror("ici");
		if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("Error: dup2 out failed"); //changer ici a la fin
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
		//dprintf(2, "fd_out= %d\n", cmd->fd_out);
	}
	while (k < elements->nb_cmd - 1)
	{
		if (k == i - 1)
		{
			//perror("on passe la");
			if (cmd->fd_in == NO_TRY_OPEN)
			{
				//perror("la aussi");
				if (dup2(elements->pipes[k][0], STDIN_FILENO) == ERROR_OPEN)
				{
					perror("Error: dup2 in failed"); //changer ici a la fin
					(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
				}
				//dprintf(2, "k = %d, elements->pipes[k][0] =%d\n", k, elements->pipes[k][0]);
				//perror("au cas ou");
			}
			//dprintf(2, "k = %d, elements->pipes[k][0] =%d\n", k, elements->pipes[k][0]);
			close(elements->pipes[k][1]);
			close(elements->pipes[k][0]);
			//dprintf(2, "1Après fermeture, pipe[%d]: read = %d, write = %d\n", k, elements->pipes[k][0], elements->pipes[k][1]);
		}
		else
		{
			close(elements->pipes[k][1]);
			close(elements->pipes[k][0]);
		}
		k++;
	}
	//dprintf(2, "derniere cmd fdin = %d fd-out = %d\n", cmd->fd_in, cmd-> fd_out);
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
	int		i;
	t_cmd	*current;

	i = 0;

	//while (i < ac - 3)
	current = elements->lst;
	elements->child_to_wait = elements->nb_cmd;
	printf("\n AVANT FORK \n\n");
	print_cmd_list(elements->lst);
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
			//perror("tt");
			child_process(i, elements, current, gc);
			//perror("tt2");
			exec_command(elements, gc, i); //faire distinction entre buil in et autre ici + rajouter cas ou une seule commande
		}
		i++;
		current = current->next;
	}
}