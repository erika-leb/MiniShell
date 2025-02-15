/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_parta.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:43:09 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/14 18:45:40 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_change_pwd(t_built *built, t_gc *gc, char *buff_old, char *buff)
{
	t_env	*head;
	t_env	*current;
	int		i;

	i = -1;
	head = NULL;
	while (built->elements->env[++i])
		ft_adder(&head, built->elements->env[i], gc);
	current = head;
	while (current)
	{
		if (strcmp(current->name, "OLDPWD") == 0)
			change_var_name(current, buff_old, gc);
		if (strcmp(current->name, "PWD") == 0)
			change_var_name(current, buff, gc);
		current = current->next;
	}
	free(buff);
	free(buff_old);
	remove_old_env(head, built, gc);
}

int	check_home_cd(t_built *built, t_gc *gc, int ch, char *buff_old)
{
	if (!built->cmd[1])
	{
		if (is_home_set(built->elements->env, gc) == TRUE)
		{
			if (ft_home_set(built, gc, ch, buff_old) == 1)
				return (1);
		}
		else
		{
			if (ft_home_not_set(built, gc, ch, buff_old) == 1)
				return (1);
		}
	}
	if (ft_change_dir(built, gc, ch, buff_old) == 1)
		return (1);
	return (0);
}

void	ft_exit_with_success(t_built *built, t_gc *gc, int ch)
{
	if (ch == 0)
	{
		built->elements->exit_status = ft_itoa(0, gc);
		return ;
	}
	else
		(gc_cleanup(gc), free_std(), exit(0));
}

void	ft_cd(t_built *built, t_gc *gc, int ch)
{
	char	*buff_old;
	char	*buff;

	if (built->cmd[1] && is_invalid_option(built->cmd[1]) == TRUE)
	{
		if (ft_invalid_option(built, gc, ch) == 1)
			return ;
	}
	buff_old = getcwd(NULL, 0);
	if (!buff_old)
	{
		if (ft_no_buff_old(built, gc, ch) == 1)
			return ;
	}
	if (check_home_cd(built, gc, ch, buff_old) == 1)
		return ;
	buff = getcwd(NULL, 0);
	if (!buff)
	{
		free(buff_old);
		ft_exit_with_success(built, gc, ch);
		return ;
	}
	ft_change_pwd(built, gc, buff_old, buff);
	ft_exit_with_success(built, gc, ch);
}
