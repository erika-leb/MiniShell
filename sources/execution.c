/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:14:40 by ele-borg          #+#    #+#             */
/*   Updated: 2024/12/01 23:36:55 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static char	*ft_strjoin(char *s1, char *s2, t_gc *gc)
{
	char	*str;
	int		i;
	int		size1;
	int		size2;
	int		j;

	size1 = ft_strlen(s1);
	size2 = ft_strlen(s2);
	str = gc_malloc(sizeof(char) * (size1 + size2 + 1), gc);
	i = 0;
	j = 0;
	if (str == 0)
		return (NULL);
	while (i < size1)
	{
		str[i] = s1[i];
		i++;
	}
	while (i < size1 + size2)
		str[i++] = s2[j++];
	str[i] = '\0';
	return (str);
}

// void	process_failed(int ac, t_pipe *var, int i)
// {
// 	if (i == 0)
// 	{
// 		close(var->pipes[i][1]);
// 		if (var->fd_in != -1)
// 		{
// 			close(var->fd_in);
// 			var->fd_in = -1;
// 		}
// 	}
// 	else if (i == ac - 4)
// 	{
// 		close(var->pipes[i - 1][0]);
// 		if (var->fd_out != -1)
// 		{
// 			close(var->fd_out);
// 			var->fd_out = -1;
// 		}
// 	}
// 	else
// 	{
// 		close(var->pipes[i][1]);
// 		close(var->pipes[i - 1][0]);
// 	}
// }

void	path_abs(char **cmd, t_element *elements, int i, t_gc *gc)
{
	int	err;

	err = 0;
	(void) i;
	if (access(cmd[0], X_OK) != 0)
	{
		write(2, "Error : command not found\n", 27);
		(free_std(), gc_cleanup(gc), exit(EXIT_FAILURE)); //autre que exit_fialure, il faut le numero en cas de cmd not found
	}
	execve(cmd[0], cmd, elements->env);
	err = errno;
	if (err == 26)
	{
		write(2, "Error : Text file busy\n", 24);
		(gc_cleanup(gc), exit(EXIT_FAILURE));
	}
}

void	path_relat(char **cmd, t_element *elements, int i, t_gc *gc)
{
	int		j;
	char	*filepath;

	j = 0;
	(void) i;
	filepath = NULL;
	// if (!elements->mypaths)
	// 	free_env(var, arg); // a quoi ca sert ??
	//perror("test1");
	while (elements->mypaths[j])
	{
		if (filepath != NULL)
			free(filepath);
		filepath = ft_strjoin(elements->mypaths[j], cmd[0], gc);
		//printf(" ")
		// if (filepath == NULL)
		// 	exit_error(var, arg, var->ac, "error strjoin");
		if (access(filepath, X_OK) == 0)
			break ;
		j++;
	}
	//perror("test2");
	if (!elements->mypaths[j]) //voir le cas ou !elements->mypaths a gerer
	{
		write(2, "Error : command not found\n", 27);
		(gc_cleanup(gc), free_std(), exit(EXIT_FAILURE));
	}
	//dprintf(2,"filepath = %s\n", filepath);
	execve(filepath, cmd, elements->env); //mettre ici un gc_cleanup ?
	//perror("test3");
}

void	exec_command(t_element *elements, t_gc *gc, int i)
{
	t_cmd	*current;
	int		j;

	j = 0;
	current = elements->lst;
	//perror("test");
	while (j++ < i && current != NULL)
		current = current->next;
	if (current->cmd[0] == NULL) // pas tout a fait !!! REVOIR CE CAS
	{
		write(2, "command not found\n", 19);
		(gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	if (current->cmd[0] && current->cmd[0][0]
		&& (current->cmd[0][0] == '/'
		|| current->cmd[0][0] == '.'))
		path_abs(current->cmd, elements, i, gc);
	else
	{
		//perror("test");
		path_relat(current->cmd, elements, i, gc);
	}
	(gc_cleanup(gc), exit(EXIT_FAILURE));
}
