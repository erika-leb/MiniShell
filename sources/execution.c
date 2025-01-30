/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:14:40 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/30 12:09:30 by ele-borg         ###   ########.fr       */
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
	int		err;

	err = 0;
	(void) i;
	if (access(cmd[0], X_OK) != 0)
	{
		err = errno;
		//write(2, "Error : command not found\n", 27);
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(cmd[0], elements, gc);
		ft_buff_error(": ", elements, gc);
		ft_buff_error(strerror(err), elements, gc);
		ft_buff_error("\n", elements, gc);
		ft_write_error(elements, gc);
		(free_std(), gc_cleanup(gc), exit(EXIT_FAILURE)); //autre que exit_fialure, il faut le numero en cas de cmd not found
	}
	if (execve(cmd[0], cmd, elements->env) == -1)
	{
		err = errno;
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(cmd[0], elements, gc);
		ft_buff_error(": ", elements, gc);
		ft_buff_error(strerror(err), elements, gc);
		ft_buff_error("\n", elements, gc);
		ft_write_error(elements, gc);
		gc_cleanup(gc), free_std(), exit(errno); //mettre ici un gc_cleanup ?
	}
	// err = errno;
	// {
	// 	ft_buff_error("minishell: ", elements, gc);
	// 	ft_buff_error(cmd[0], elements, gc);
	// 	ft_buff_error(strerror(err), elements, gc);
	// 	ft_write_error(elements, gc);
	// 	//write(2, "Error : Text file busy\n", 24);
	// 	(gc_cleanup(gc), exit(EXIT_FAILURE));
	// }

	// if (err == 26)
	// {
	// 	ft_buff_error("minishell: ", elements, gc);
	// 	ft_buff_error(cmd[0], elements, gc);
	// 	ft_buff_error(": Text file busy\n", elements, gc);
	// 	ft_write_error(elements, gc);
	// 	//write(2, "Error : Text file busy\n", 24);
	// 	(gc_cleanup(gc), exit(EXIT_FAILURE));
	// }
	// else
	// {
	// 	perror("test"); // message a mettre a jour
	// 	gc_cleanup(gc), free_std(), exit(errno); //mettre ici un gc_cleanup ?
	// }
}

void	write_all_err_mess(char *str1, char *str2, t_element *elements, t_gc *gc)
{
	ft_buff_error("minishell: ", elements, gc);
	ft_buff_error(str1, elements, gc);
	ft_buff_error(str2, elements, gc);
	ft_write_error(elements, gc);
}

void	path_relat(char **cmd, t_element *elements, int i, t_gc *gc)
{
	int		j;
	char	*filepath;
	int		err;

	j = 0;
	(void) i;
	filepath = NULL;
	if (!elements->mypaths)
	{
		write_all_err_mess(cmd[0], ": command not found\n", elements, gc);
		gc_cleanup(gc), free_std(), exit(EXIT_FAILURE);
	}
	//perror("test1");
	while (elements->mypaths[j])
	{
		if (filepath != NULL)
			gc_remove(gc, filepath);
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
		err = errno;
		//perror("alex");
		write_all_err_mess(cmd[0], ": command not found\n", elements, gc);
		// ft_buff_error("minishell: ", elements, gc);
		// ft_buff_error(cmd[0], elements, gc);
		// ft_buff_error(": command not found\n", elements, gc);
		// ft_write_error(elements, gc);
		//write(2, "Error : command not found\n", 27);
		// ft_buff_error("minishell: ", elements, gc);
		// ft_buff_error(cmd[0], elements, gc);
		// ft_buff_error(": ", elements, gc);
		// ft_buff_error(strerror(err), elements, gc);
		// ft_buff_error("\n", elements, gc);
		// ft_write_error(elements, gc);
		// (gc_cleanup(gc), free_std(), exit(EXIT_FAILURE));
		//(free_std(), exit(EXIT_FAILURE));
		gc_cleanup(gc), free_std(), exit(127);
	}
	//perror("tara");
	// printf("\n AVANT EXEC \n\n");
	// print_cmd_list(elements->lst);
	//dprintf(2,"filepath = %s\n", filepath);
	//check_fds();
	//printf("errno dans child= %d\n", errno);
	//ft_exec_signal(gc);
	//dprintf(2, "file = %s\n", cmd[0]);
	//dprintf(2, "file = %s\n", cmd[1]);
	if (execve(filepath, cmd, elements->env) == -1)
	{
		err = errno;
		//perror("willow");
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(cmd[0], elements, gc);
		ft_buff_error(": ", elements, gc);
		ft_buff_error(strerror(err), elements, gc);
		ft_buff_error("\n", elements, gc);
		ft_write_error(elements, gc);
		gc_cleanup(gc), free_std(), exit(errno); //mettre ici un gc_cleanup ?
	}//perror("test3");
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
	if (current->cmd[0] == NULL) // pas tout a fait !!! REVOIR CE CAS INUTILE JE CROIS
	{
		write_all_err_mess(current->cmd[0], ": command not found\n", elements, gc);
		//write_all_err_mess(cmd[0], ": command not found\n", elements, gc);
		//write(2, "command not found\n", 19); // a changer aussi
		(gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	//ft_exec_signal(gc);
	//perror("marty");
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
