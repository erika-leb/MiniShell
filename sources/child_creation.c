/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:06:24 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/10 19:11:45 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	uniq_case(t_element *elements, t_cmd *cmd, t_gc *gc) //incrementer SHLV ici si cmd[0]=..../minishell cmp grace a 	if (access(cmd[0], X_OK) != 0)
{
	if (cmd->fd_in == ERROR_OPEN || cmd->fd_in == ERROR_OPEN)
	(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	if (cmd->fd_in >= 0)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == ERROR_OPEN)
		{
			perror("Error: dup2 in failed"); //changer ici a la fin
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
	if (cmd->fd_out >= 0) // possible de mettre les deux conditions a la suite
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == ERROR_OPEN)
		{
			perror("Error: dup2 out failed"); //changer ici a la fin
			(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
		}
	}
}

void	child_process(int i, t_element *elements, t_cmd *cmd, t_gc *gc)
{
	//printf("i = %d\n", i);
	if (cmd->fd_out == ERROR_OPEN || cmd->fd_in == ERROR_OPEN)
	{
		//perror("roken");
		(close_pipes(elements), gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	if (i == 0 && elements->nb_cmd == 1)
	{
		//perror("cyan");
		uniq_case(elements, cmd, gc);
		//perror("on est ici");
	}
	else
	{
		//perror("PR");
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
	close_other_redir(i, elements); //pbm ?
}

int	no_child_events(t_element *elements, t_gc *gc, t_cmd *current)
{
	if (elements->nb_cmd == 1 && !elements->lst->cmd[0])
	{
		elements->child_to_wait = 0;
		return (1);
	}
	if (elements->lst->cmd[0] && ft_strcmp(elements->lst->cmd[0], "\n") == 0)
	{
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
		return (1);
	}
	if (elements->nb_cmd == 1 && (ft_strcmp(elements->lst->cmd[0], "exit") == 0
			|| ft_strcmp(elements->lst->cmd[0], "export") == 0
			|| ft_strcmp(elements->lst->cmd[0], "unset") == 0
			|| ft_strcmp(elements->lst->cmd[0], "cd") == 0))
		return (built_in_no_child(elements, gc), 1);
	return (0);
}

void	hedge_child_cases(t_element *elements, t_gc *gc, t_cmd	*current)
{
	t_file 	*redir;

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

void	child_creation(t_element *elements, t_gc *gc)
{
	int		i;
	t_cmd	*current;

	i = 0;
	current = elements->lst;
	elements->child_to_wait = elements->nb_cmd;
	//printf("current-> cmd = %s\n", current->cmd[0]);
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
		{
			elements->pid_arr[i] = fork();
			if (elements->pid_arr[i] == -1)
			{
				elements->child_to_wait = elements->child_to_wait - 1; //a verifier si pas de pbm apres
				perror("fork failed");
			}
			if (elements->pid_arr[i] == 0)
			{
				hedge_child_cases(elements, gc, current);
				child_process(i, elements, current, gc);
				// printf("\n APRES FORK \n\n");
				// print_cmd_list(elements->lst);
				if (is_built_in(current->cmd[0]) == TRUE)
					ft_built_in(elements, current->cmd, gc);
				else
					exec_command(elements, gc, i);
				if (g_signal == 13)
				{
					g_signal = 0;
					gc_cleanup(gc);
					free_std();
					//perror("liloo");
					exit(128 + 13); // Quitte la boucle
				}
			}
		}
		else
			elements->child_to_wait = elements->child_to_wait - 1;
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
