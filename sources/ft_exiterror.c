/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exiterror.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:22:24 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/14 11:54:22 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_exitfail(t_built *built, t_gc *gc)
{
	char	*code;

	code = built->cmd[1];
	write(1, "exit\n", 6);
	ft_buff_error("minishell: exit: ", built->elements, gc);
	ft_buff_error(code, built->elements, gc);
	ft_buff_error(": numeric argument required\n", built->elements, gc);
	ft_write_error(built->elements, gc);
	(gc_cleanup(gc), free_std(), exit(2));
}

int	ft_error_many(t_built *built, t_gc *gc, int ch)
{
	write(1, "exit\n", 6);
	ft_buff_error("minishell: exit: too many arguments\n", built->elements, gc);
	ft_write_error(built->elements, gc);
	if (ch == 0)
	{
		built->elements->exit_status = ft_itoa(1, gc);
		return (1);
	}
	else
		(gc_cleanup(gc), free_std(), exit(1));
	return (0);
}

int	ft_checkexit(t_built *built, t_gc *gc, int ch)
{
	if (!built->cmd[1])
		(gc_cleanup(gc), free_std(), exit(0));
	if (!ft_isdigitexit(built->cmd[1]))
		ft_exitfail(built, gc);
	if (built->cmd[2])
	{
		if (ft_error_many(built, gc, ch) == 1)
			return (0);
	}
	return (1);
}
