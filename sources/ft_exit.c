/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:19:30 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/13 15:16:46 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static void	ft_initexit(const char *str, int *i, int *neg, long *res)
{
	*i = 0;
	*neg = 1;
	*res = 0;
	while ((str[*i] >= '\t' && str[*i] <= '\r') || str[*i] == ' ')
		(*i)++;
}

static void	ft_fillexit(t_built *built, int *i, int neg, long *res, t_gc *gc)
{
	int	curr_digit;

	while (built->cmd[1][*i] >= '0' && built->cmd[1][*i] <= '9')
	{
		curr_digit = built->cmd[1][*i] - '0';
		if ((neg == 1 && *res > (LONG_MAX - curr_digit) / 10)
			|| (neg == -1 && *res > (LONG_MIN + curr_digit) / -10))
			ft_exitfail(built, gc);
		*res = (*res) * 10 + curr_digit;
		(*i)++;
	}
	while ((built->cmd[1][*i] >= '\t'
		&& built->cmd[1][*i] <= '\r') || built->cmd[1][*i] == ' ')
		(*i)++;
}

int	ft_isdigitexit(char *num)
{
	int	i;

	i = 1;
	if (!(num[0] == '-' || num[0] == '+' || (num[0] >= '0' && num[0] <= '9')))
		return (0);
	while (num[i])
	{
		if (!(num[i] >= '0' && num[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_built *built, t_gc *gc, int ch) //ch = 0 means 0 child , 1 mean un enfant
{
	int		i;
	int		neg;
	long	res;

	if (!ft_checkexit(built, gc, ch))
		return (0);
	ft_initexit(built->cmd[1], &i, &neg, &res);
	if (built->cmd[1][i] == '-')
	{
		neg = -1;
		i++;
	}
	else if (built->cmd[1][i] == '+')
		i++;
	if (!(built->cmd[1][i] >= '0' && built->cmd[1][i] <= '9'))
		ft_exitfail(built, gc);
	ft_fillexit(built, &i, neg, &res, gc);
	if (built->cmd[1][i])
		ft_exitfail(built, gc);
	if (neg == -1)
		res = -res;
	write(1, "exit\n", 6);
	(gc_cleanup(gc), free_std(), exit((unsigned int)res % 256));
}
