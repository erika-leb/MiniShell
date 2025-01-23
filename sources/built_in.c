/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 17:03:01 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/23 16:53:14 by ele-borg         ###   ########.fr       */
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
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (TRUE);
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

	else if (ft_strcmp(cmd[0], "pwd") == 0)
		ft_pwd(gc);
	// int i;
	// int s_arr;
	// s_arr = ft_arr_size(elements->env);
	// printf("s = %d\n", s_arr);
	// i = 0;
	// while( i <= s_arr)
	// {
	// 	printf("element2->env %i = %s\n", i, elements->env[i]);
	// 	//printf("myenv %i = %s\n", i, elements->env[i]);
	// 	i++;
	// }
	(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
}

void	built_in_no_child(t_element *elements, t_gc *gc)
{
	t_built	*built;
		//perror("passasge ici");
	built = gc_malloc(sizeof(t_built), gc);
	built->cmd = elements->lst->cmd;
	built->elements = elements;
	if (ft_strcmp(built->cmd[0], "exit") == 0)
		ft_exit(built, gc);
	else if (ft_strcmp(built->cmd[0], "export") == 0)
		ft_export(elements, built->cmd, gc);
	else if (ft_strcmp(built->cmd[0], "unset") == 0)
		ft_unset(elements, built->cmd, gc);
}
