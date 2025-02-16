/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_partb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:16:17 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/15 18:26:23 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_invalid_option(t_built *built, t_gc *gc, int ch)
{
	ft_buff_error("minishell: cd: ", built->elements, gc);
	ft_buff_error(ft_substr(built->cmd[1], 0, 2, gc), built->elements, gc);
	ft_buff_error(": invalid option\n", built->elements, gc);
	ft_write_error(built->elements, gc);
	if (ch == 0)
	{
		built->elements->exit_status = ft_itoa(2, gc);
		return (1);
	}
	else
		(gc_cleanup(gc), free_std(), exit(2));
	return (0);
}

int	ft_no_buff_old(t_built *built, t_gc *gc, int ch)
{
	ft_buff_error("cd: error retrieving current ", built->elements, gc);
	ft_buff_error("directory: getcwd: cannot access ", built->elements, gc);
	ft_buff_error("parent directories: No such file", built->elements, gc);
	ft_buff_error(" or directory\n", built->elements, gc);
	ft_write_error(built->elements, gc);
	if (ch == 0)
	{
		built->elements->exit_status = ft_itoa(2, gc);
		return (1);
	}
	else
		(gc_cleanup(gc), free_std(), exit(2));
	return (0);
}

int	ft_home_set(t_built *built, t_gc *gc, int ch, char	*buff_old)
{
	if (cd_home(built, gc) == 1)
	{
		cd_error(built, gc);
		built->elements->exit_status = ft_itoa(1, gc);
		free(buff_old);
		if (ch == 0)
		{
			built->elements->exit_status = ft_itoa(1, gc);
			return (1);
		}
		else
			(gc_cleanup(gc), free_std(), exit(1));
	}
	free(buff_old);
	if (ch == 0)
	{
		built->elements->exit_status = ft_itoa(0, gc);
		return (1);
	}
	else
		(gc_cleanup(gc), free_std(), exit(0));
	return (0);
}

int	ft_home_not_set(t_built *built, t_gc *gc, int ch, char	*buff_old)
{
	if (is_home_set(built->elements->env, gc) == FALSE)
	{
		write(2, "minishell: cd : HOME not set\n", 30);
		free(buff_old);
		if (ch == 0)
		{
			built->elements->exit_status = ft_itoa(1, gc);
			return (1);
		}
		else
			(gc_cleanup(gc), free_std(), exit(1));
	}
	else if (is_home_set(built->elements->env, gc) == 3)
	{
		free(buff_old);
		if (ch == 0)
		{
			built->elements->exit_status = ft_itoa(0, gc);
			return (1);
		}
		else
			(gc_cleanup(gc), free_std(), exit(0));
	}
	return (0);
}

int	ft_change_dir(t_built *built, t_gc *gc, int ch, char	*buff_old)
{
	if (access(built->cmd[1], F_OK) == 0 && built->cmd[2])
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		free(buff_old);
		if (ch == 0)
		{
			built->elements->exit_status = ft_itoa(1, gc);
			return (1);
		}
		else
			(gc_cleanup(gc), free_std(), exit(1));
	}
	if (chdir(built->cmd[1]) != 0)
	{
		cd_error(built, gc);
		free(buff_old);
		if (ch == 0)
		{
			built->elements->exit_status = ft_itoa(1, gc);
			return (1);
		}
		else
			(gc_cleanup(gc), free_std(), exit(1));
	}
	return (0);
}
