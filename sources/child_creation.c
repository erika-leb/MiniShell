/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:06:24 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/21 17:33:37 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

void	close_other_redir(int i, t_element *elements) // fonction inutile ?
{
	t_cmd	*cmd;
	int		k;

	k = 0;
	cmd = elements->lst;
	while (cmd)
	{
		if (k != i)
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
		}
		k++;
		cmd=cmd->next;
	}
}

void	first_cmd_with_valid_infile(t_element *elements, t_cmd *cmd, t_gc *gc)
{
	if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
	{
		perror("Error: dup2 in failed"); //changer ici a la fin
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	//(close(elements->pipes[0][0]), close(elements->pipes[0][1]));
}

void	last_cmd_with_valid_outfile(t_element *elements, t_cmd *cmd, t_gc *gc)
{
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
		first_cmd_with_valid_infile(elements, cmd, gc);
	// {
	// 	if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
	// 	{
	// 		perror("Error: dup2 in failed"); //changer ici a la fin
	// 		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	// 	}
	// }



	if (i == elements->nb_cmd - 1 && cmd->fd_out >= 0) // cas dernier
		last_cmd_with_valid_outfile(elements, cmd, gc);
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
			(close(elements->pipes[k][1]), close(elements->pipes[k][0]));
		k++;
	}
}

void check_fds(const char *context)
{
    struct stat statbuf;

    dprintf(2, "Checking fds in %s\n", context);

    if (fcntl(STDIN_FILENO, F_GETFD) != -1)
    {
        dprintf(2, "stdin is open\n");
        if (fstat(STDIN_FILENO, &statbuf) == 0)
        {
            dprintf(2, "stdin is associated with inode: %ld\n", statbuf.st_ino);
        }
        else
        {
            perror("fstat stdin");
        }
    }
    else
    {
        dprintf(2, "stdin is closed\n");
    }

    if (fcntl(STDOUT_FILENO, F_GETFD) != -1)
    {
        dprintf(2, "stdout is open\n");
        if (fstat(STDOUT_FILENO, &statbuf) == 0)
        {
            dprintf(2, "stdout is associated with inode: %ld\n", statbuf.st_ino);
        }
        else
        {
            perror("fstat stdout");
        }
    }
    else
    {
        dprintf(2, "stdout is closed\n");
    }
}

void	uniq_case(t_element *elements, t_cmd *cmd, t_gc *gc)
{
	// if (is_built_in(current->cmd[0]) == TRUE)
	// 	ft_built_in(elements, current->cmd, gc); // a voir
	// else
		//exec_uniq_command(elements, gc);
	//perror("ic");
	if (cmd->fd_in == ERROR_OPEN || cmd->fd_in == ERROR_OPEN)
	(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
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
	if (cmd->fd_out >= 0) // possible de mettre les deux conditions a la suite
	{
		//cmd->fd_out = STDOUT_FILENO;
		//perror("ici");
		if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("Error: dup2 out failed"); //changer ici a la fin
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
		//dprintf(2, "fd_out= %d\n", cmd->fd_out);
	}

	//check_fds("child");
	// close(elements->pipes[0][1]);
	// close(elements->pipes[0][0]);
}

void	child_process(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	//printf("i = %d\n", i);
	if (cmd->fd_out == ERROR_OPEN || cmd->fd_in == ERROR_OPEN)
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	close_other_redir(i, elements);
	if (i == 0 && elements->nb_cmd == 1)
	{
		uniq_case(elements, cmd, gc);
		//perror("on est ici");
	}
	else
		all_cases(i, elements, cmd, gc);
	// if (i == 0)
	// 	first_case(i, elements, cmd, gc);
	// else if (i == elements->nb_cmd - 1)
	// 	last_case(i, elements, cmd, gc);
	// else
	// 	other_cases(i, elements, cmd, gc);
	// printf("\n CAS 5 \n\n");
	// print_cmd_list(elements->lst);



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


	// printf("\n CAS 6 \n\n");
	// print_cmd_list(elements->lst);
}

int		is_built_in(char *cmd)
{
	if (ft_strcmp(cmd, "exit") == 0)
		return (TRUE);
	if (ft_strcmp(cmd, "echo") == 0)
		return (TRUE);
	else if (ft_strcmp(cmd, "export") == 0)
		return (TRUE);
	// else if (ft_strcmp(cmd, "cd") == 0)
	// 	return (TRUE);
	// else if (ft_strcmp(cmd, "pwd") == 0)
	// 	return (TRUE);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (TRUE);
	else if (ft_strcmp(cmd, "env") == 0)
		return (TRUE);
	else
		return (FALSE);
}

//Creer une super structure compose de elements etcmd d'un cote, et gc que adri
//recevra en argument.
void	ft_built_in(t_element *elements, char **cmd, t_gc *gc)
{
	t_built	*built;

	built = gc_malloc(sizeof(t_built), gc);
	built->cmd = cmd;
	built->elements = elements;
	if (ft_strcmp(cmd[0], "exit") == 0)
		ft_exit(built, gc);
	if (ft_strcmp(cmd[0], "echo") == 0)
		ft_echo(cmd, gc);
	else if (ft_strcmp(cmd[0], "export") == 0)
	 	ft_export(elements, cmd, gc);
	else if (ft_strcmp(cmd[0], "env") == 0)
		ft_env(elements->env, cmd, gc);
	else if (ft_strcmp(cmd[0], "unset") == 0)
		ft_unset(elements, cmd, gc);
	// else if (ft_strcmp(cmd[0], "cd") == 0)

	// else if (ft_strcmp(cmd[0], "pwd") == 0)
	(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
}

void	child_creation(t_element *elements, t_gc *gc) //prevoir la cas ou cmd[0]=NULL (mais on a des redir)
{
	int		i;
	t_cmd	*current;
	t_built	*built;

	i = 0;
	//while (i < ac - 3)
	current = elements->lst;
	elements->child_to_wait = elements->nb_cmd;
	//printf("current-> cmd = %s\n", current->cmd[0]);
	// printf("\n AVANT FORK \n\n");
	// print_cmd_list(elements->lst);
	//perror("la");
	//printf("nb cmd = %d\n", elements->nb_cmd);
	if (ft_strcmp(elements->lst->cmd[0], "\n") == 0)
	{
		//perror("on entre ic ?");
		if (current->fd_in >= 0)
			close(current->fd_in);
		if (current->fd_out >= 0)
			close(current->fd_out);
		//exec_command(elements, gc, 0);
		return ;
	}
	if (elements->nb_cmd == 1 && ft_strcmp(elements->lst->cmd[0], "exit") == 0)
	{
		built = gc_malloc(sizeof(t_built), gc);
		built->cmd = elements->lst->cmd;
		built->elements = elements;
		ft_exit(built, gc);
		return ;
	}
	while (i < elements->nb_cmd) //voir a partir de la
	{
		elements->pid_arr[i] = fork();
		if (elements->pid_arr[i] == -1)
		{
			elements->child_to_wait = elements->child_to_wait - 1; //a verifier si pas de pbm apres
			perror("fork failed");
		}
		if (elements->pid_arr[i] == 0)
		{
			//perror("tt");
			if (!current->cmd[0])
				(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
			child_process(i, elements, current, gc);
			// printf("\n APRES FORK \n\n");
			// print_cmd_list(elements->lst);
			//perror("tt2");
			if (is_built_in(current->cmd[0]) == TRUE)
			{
				//perror("ets1");
				ft_built_in(elements, current->cmd, gc);  // a voir
			}
			else
			{
				//perror("ets2");
				exec_command(elements, gc, i); // faire distinction entre buil in et autre ici + rajouter cas ou une seule commande
			}
		}
		i++;
		current = current->next;
	}
}
