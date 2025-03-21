/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:53:57 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/16 21:43:37 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_istok_2(char *av2)
{
	if (!ft_strcmp(av2, "<") || !ft_strcmp(av2, ">") || !ft_strcmp(av2, "<<")
		|| !ft_strcmp(av2, ">>"))
		return (1);
	return (0);
}

static void	ft_init_values(int *i, int *p_max, t_element *elements)
{
	(*i) = 0;
	elements->child_to_wait = 0;
	(*p_max) = 0;
}

void	hedge_case_1(char **tab, t_element *elements, t_gc *gc)
{
	int	i;
	int	p_max;

	ft_init_values(&i, &p_max, elements);
	while (1)
	{
		if (ft_strcmp(tab[p_max], "|") == 0 && tab[p_max + 1]
			&& !ft_strcmp(tab[p_max + 1], "|"))
			break ;
		if (tab[p_max] && ft_istok_(tab[p_max]) && tab[p_max + 1]
			&& ft_istok_(tab[p_max + 1]))
			break ;
		if ((tab[p_max] && ft_istok_(tab[p_max]) && tab[p_max + 1]
				&& !ft_strcmp(tab[p_max + 1], "|")) || (ft_istok_(tab[p_max])
				&& !tab[p_max + 1]) || !ft_strcmp(tab[1], "|"))
			break ;
		p_max++;
	}
	while (i < p_max - 1)
	{
		if (ft_strcmp(tab[i], "<<") == 0)
			ft_open_heredoc_error(tab[i + 1], elements, gc);
		i++;
	}
}

static t_arg	*ft_init_arg(t_gc *gc, char **tab, t_cmd **lst)
{
	t_arg	*arg;

	arg = NULL;
	arg = gc_malloc(sizeof(t_arg), gc);
	arg->tab = tab;
	arg->lst = lst;
	arg->last_i = -1;
	arg->i = 0;
	arg->j = 0;
	arg->k = 0;
	return (arg);
}

void	lexing(char **tab, t_cmd **lst, t_element *elements, t_gc *gc)
{
	t_arg	*arg;

	arg = ft_init_arg(gc, tab, lst);
	while (tab[arg->i])
	{
		if (ft_strcmp(tab[arg->i], "|") == 0)
		{
			create_chain(arg, elements, gc);
			arg->last_i = arg->i;
		}
		(arg->i)++;
	}
	(arg->i)--;
	create_chain(arg, elements, gc);
	if (tab && tab[0] && ft_strcmp(tab[0], "\n") == 0)
		hedge_case_1(tab, elements, gc);
	else
		handle_redir(lst, elements, gc);
	ft_write_error(elements, gc);
}
