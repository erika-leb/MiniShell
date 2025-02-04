/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 15:11:54 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/04 15:19:42 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

int	is_invalid_option(char *str)
{
	if (str[0] == '-' && str[1])
	{
		//perror("bill");
		return (TRUE);
	}
	//perror("boule");
	return (FALSE);
}

void	ft_pwd(char **cmd, t_element *elements, t_gc *gc)
{
	char	*buff;
	char	*res;
	int		size;

	if (cmd[1] && is_invalid_option(cmd[1]) == TRUE)
	{
		ft_buff_error("minishell: pwd: ", elements, gc);
		ft_buff_error(ft_substr(cmd[1], 0, 2, gc), elements, gc);
		ft_buff_error(": invalid option\n", elements, gc);
		//ft_write_error(elements, gc);
		(ft_write_error(elements, gc), gc_cleanup(gc), free_std(), exit(2));
	}
	buff = getcwd(NULL, 0);
	if (!buff)
	{
		ft_buff_error("pwd: error retrieving current directory", elements, gc);
		ft_buff_error(": getcwd: cannot access parent ", elements, gc);
		ft_buff_error("directories: No such file", elements, gc);
		ft_buff_error(" or directory\n", elements, gc);
		//ft_write_error(elements, gc);
		(ft_write_error(elements, gc), gc_cleanup(gc), free_std(), exit(1));
		return ;
	}
	res = ft_strjoin_(buff, "\n", gc);
	size = ft_strlen(res);
	write(1, res, size);
	//free(buff);
	(free(buff), gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
}
