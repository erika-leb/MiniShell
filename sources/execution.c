/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:14:40 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/13 15:12:14 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	path_abs(char **cmd, t_element *elements, t_gc *gc)
{
	if (access(cmd[0], X_OK) != 0)
	{
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(cmd[0], elements, gc);
		ft_buff_error(": ", elements, gc);
		ft_buff_error(strerror(errno), elements, gc);
		ft_buff_error("\n", elements, gc);
		ft_write_error(elements, gc);
		(free_std(), gc_cleanup(gc), exit(127));
	}
	if (is_directory(cmd[0]) != 0)
	{
		write_all_err_mess(cmd[0], ": Is a directory\n", elements, gc);
		(free_std(), gc_cleanup(gc), exit(126));
	}
	if (execve(cmd[0], cmd, elements->env) == -1)
	{
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(cmd[0], elements, gc);
		ft_buff_error(": ", elements, gc);
		ft_buff_error(strerror(errno), elements, gc);
		ft_buff_error("\n", elements, gc);
		ft_write_error(elements, gc);
		gc_cleanup(gc), free_std(), exit(errno);
	}
}

static void	ft_no_path(char **cmd, t_element *elements, t_gc *gc)
{
	if (!elements->mypaths)
	{
		write_all_err_mess(cmd[0], ": command not found\n", elements, gc);
		gc_cleanup(gc), free_std(), exit(EXIT_FAILURE);
	}
}

void	ft_cmd_not_fnd(char **cmd, int j, t_element *elements, t_gc *gc)
{
	if (!elements->mypaths[j] || cmd[0][0] == '\0')
	{
		write_all_err_mess(cmd[0], ": command not found\n", elements, gc);
		gc_cleanup(gc), free_std(), exit(127);
	}
}

void	path_relat(char **cmd, t_element *elements, t_gc *gc)
{
	int		j;
	char	*filepath;

	j = -1;
	filepath = NULL;
	ft_no_path(cmd, elements, gc);
	while (elements->mypaths[++j])
	{
		if (filepath != NULL)
			gc_remove(gc, filepath);
		filepath = ft_strjoin(elements->mypaths[j], cmd[0], gc);
		if (access(filepath, X_OK) == 0)
			break ;
	}
	ft_cmd_not_fnd(cmd, j, elements, gc);
	if (execve(filepath, cmd, elements->env) == -1)
	{
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(cmd[0], elements, gc);
		ft_buff_error(": ", elements, gc);
		ft_buff_error(strerror(errno), elements, gc);
		ft_buff_error("\n", elements, gc);
		ft_write_error(elements, gc);
		gc_cleanup(gc), free_std(), exit(errno);
	}
}

void	exec_command(t_element *elements, t_gc *gc, int i)
{
	t_cmd	*current;
	int		j;

	j = 0;
	current = elements->lst;
	while (j++ < i && current != NULL)
		current = current->next;
	if (current->cmd[0] == NULL)
	{
		write_all_err_mess(current->cmd[0], ": command not found\n", elements, gc);
		(gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	if (current->cmd[0] && current->cmd[0][0]
		&& (current->cmd[0][0] == '/'
		|| current->cmd[0][0] == '.' || current->cmd[0][0] == '?'))
			path_abs(current->cmd, elements, gc);
	else
		path_relat(current->cmd, elements, gc);
	(gc_cleanup(gc), exit(EXIT_FAILURE));
}
