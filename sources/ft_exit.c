/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:19:30 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/07 17:10:31 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

//Seul le code d'erreur du dernier enfant compte

static void	ft_exitfail(t_built *built, t_gc *gc)
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

static void	ft_initexit(const char *str, int *i, int *neg, long *res)
{
	*i = 0;
	*neg = 1;
	*res = 0;
	while ((str[*i] >= '\t' && str[*i] <= '\r') || str[*i] == ' ')
		(*i)++;
}
//Duppliquer la logique de neg dans ft_fillexit pour diminuer argument de 1
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
		(gc_cleanup(gc), free_std(), exit(1));//Je crois qu'il faut pas exit si on est dans le parent (UNIQUEMENT)
	//S'inspirer de cd et unset
	return (0);
}

int	ft_isdigitexit(char *num)
{
	int	i;

	i = 1;
	//perror("haha");
	//Probleme de logique basique a regler
	if (!(num[0] == '-' || num[0] == '+' || (num[0] >= '0' && num[0] <= '9')))
	{
		//perror("scooby");
		return (0);
	}
	//perror("haha2");
	while (num[i])
	{
		if (!(num[i] >= '0' && num[i] <= '9'))
			return (0);
		i++;
	}
	//perror("haha3");
	return (1);
}

int	ft_exit(t_built *built, t_gc *gc, int ch) //ch = 0 means 0 child , 1 mean un enfant
{
	int		i;
	int		neg;
	long	res;

	//perror("lol");
	if (!built->cmd[1])
		(gc_cleanup(gc), free_std(), exit(0));
	//perror("lol2");
	if (!ft_isdigitexit(built->cmd[1]))
	{
		//perror("ce essage apparait si l arg n est pas un nombre");
		ft_exitfail(built, gc);
	}
	//perror("lol3");
	//dprintf(2, "ch = %d\n", ch);
	if (built->cmd[2])
	{
		//perror("el senor de la la noche");
		if (ft_error_many(built, gc, ch) == 1)
			return 0;
		//perror("bajar el infierno");
	}
	ft_initexit(built->cmd[1], &i, &neg, &res);
	if (built->cmd[1][i] == '-')
	{
		neg = -1;
		i++;
	}
	else if (built->cmd[1][i] == '+')
		i++;
	//POUR ERIKA : Dans exitfail tu ajoute un "exit" dans le buffer a afficher
	if (!(built->cmd[1][i] >= '0' && built->cmd[1][i] <= '9'))
		ft_exitfail(built, gc);
	ft_fillexit(built, &i, neg, &res, gc);
	//POUR ERIKA : Idem
	if (built->cmd[1][i])
		ft_exitfail(built, gc);
	if (neg == -1)
		res = -res;
	write(1, "exit\n", 6);
	//POUR ERIKA : Cas sans erreur : Ici tu peux ajouter un write exit avant d'exit.
	(gc_cleanup(gc), free_std(), exit((unsigned int)res % 256));
}
