/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:06:24 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/28 12:28:03 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"



void	uniq_case(t_element *elements, t_cmd *cmd, t_gc *gc) //incrementer SHLV ici si cmd[0]=..../minishell cmp grace a 	if (access(cmd[0], X_OK) != 0)
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
		perror("ici");
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
	{
		//perror("roken");
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	close_other_redir(i, elements);
	if (i == 0 && elements->nb_cmd == 1)
	{
		//perror("cyan");
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

int	no_child_events(t_element *elements, t_gc *gc, t_cmd *current)
{
	if (elements->nb_cmd == 1 && !elements->lst->cmd[0])
	{
		//perror("ici est on");
		elements->child_to_wait = 0;
		return (1);
	}
	if (elements->lst->cmd[0] && ft_strcmp(elements->lst->cmd[0], "\n") == 0)
	{
		//perror("on entre ic ?");
		if (current->fd_in >= 0)
		{
			close(current->fd_in);
			current->fd_in = CLOSED;
		}
		if (current->fd_out >= 0)
		{
			close(current->fd_out);
			current->fd_out = CLOSED;
		}
		//exec_command(elements, gc, 0);
		return (1);
	}
	if (elements->nb_cmd == 1 && (ft_strcmp(elements->lst->cmd[0], "exit") == 0
			|| ft_strcmp(elements->lst->cmd[0], "export") == 0
			|| ft_strcmp(elements->lst->cmd[0], "unset") == 0
			|| ft_strcmp(elements->lst->cmd[0], "cd") == 0))
	{
		built_in_no_child(elements, gc);
		return (1);
	}
	return (0);
}

void	hedge_child_cases(t_element *elements, t_gc *gc, t_cmd	*current)
{
	t_file 	*redir;

	//perror("waha");
	if (!current->cmd[0])
		(close_pipes(elements), gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
	redir = current->redir;
	while(redir)
	{
		if (ft_strncmp("$\n", redir->name, 2) == 0)
			(close_pipes(elements), gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
		redir = redir->next;
	}
}

void	child_creation(t_element *elements, t_gc *gc) //prevoir la cas ou cmd[0]=NULL (mais on a des redir)
{
	int		i;
	t_cmd	*current;
	// t_file 	*redir;

	i = 0;
	//while (i < ac - 3)
	current = elements->lst;
	elements->child_to_wait = elements->nb_cmd;
	//printf("current-> cmd = %s\n", current->cmd[0]);
	// printf("\n AVANT FORK \n\n");
	// print_cmd_list(elements->lst);
	//perror("la");
	//printf("nb cmd = %d\n", elements->nb_cmd);
	if (no_child_events(elements, gc, current) == 1)
	{
		//perror("gruber");
		elements->child_to_wait = 0;
		return ;
	}
	// if (elements->nb_cmd == 1 && !elements->lst->cmd[0])
	// {
	// 	elements->child_to_wait = 0;
	// 	return ;
	// }
	// if (ft_strcmp(elements->lst->cmd[0], "\n") == 0)
	// {
	// 	//perror("on entre ic ?");
	// 	if (current->fd_in >= 0)
	// 		close(current->fd_in);
	// 	if (current->fd_out >= 0)
	// 		close(current->fd_out);
	// 	//exec_command(elements, gc, 0);
	// 	return ;
	// }
	// if (elements->nb_cmd == 1 && (ft_strcmp(elements->lst->cmd[0], "exit") == 0 || ft_strcmp(elements->lst->cmd[0], "export") == 0 || ft_strcmp(elements->lst->cmd[0], "unset") == 0))
	// 	built_in_no_child(elements, gc);
	ft_handle_signal(1, gc);
	while (i < elements->nb_cmd) //voir a partir de la
	{
		if (current->active == TRUE)
		{
			elements->pid_arr[i] = fork();
			if (elements->pid_arr[i] == -1)
			{
				elements->child_to_wait = elements->child_to_wait - 1; //a verifier si pas de pbm apres
				perror("fork failed");
			}
			//ft_ignore_signal(gc);
			if (elements->pid_arr[i] == 0)
			{
				//perror("tt");
				hedge_child_cases(elements, gc, current);
				// if (!current->cmd[0])
				// 	(close_pipes(elements), gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
				// redir = current->redir;
				// while(redir)
				// {
				// 	if (ft_strncmp("$\n", redir->name, 2) == 0)
				// 		(close_pipes(elements), gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
				// 	redir = redir->next;
				// }
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
		}
		else
			elements->child_to_wait = elements->child_to_wait - 1;
		ft_handle_signal(0, gc);
		i++;
		current = current->next;
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
