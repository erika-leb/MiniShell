/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/15 18:14:00 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**ft_filladder(t_exp *xx, t_env *head, t_element *element, t_gc *gc)
{
	char	**adder;

	adder = ft_ltoa(head, gc);
	gc_remove(gc, head);
	gc_remove(gc, element->env);
	element->env = adder;
	if (xx->flag == 0)
	{
		element->exit_status = ft_itoa(0, gc);
		xx->code = 0;
	}
	return (adder);
}

static void	ft_paramex(t_exp *xx, t_element *element, t_gc *gc)
{
	element->exit_status = ft_itoa(1, gc);
	xx->code = 1;
	xx->flag = 1;
}

static void	ft_initexport(t_exp **xx, t_env **head, int *i, t_gc *gc)
{
	*xx = gc_malloc(sizeof(t_exp), gc);
	*head = NULL;
	*i = -1;
}

static void	ft_attribute(t_exp **xx, char **argv, int ch)
{
	(*xx)->argv = argv;
	(*xx)->ch = ch;
	(*xx)->flag = 0;
	(*xx)->code = 0;
}

char	**ft_export(t_element *element, char **argv, t_gc *gc, int ch)
{
	t_env	*head;
	char	**adder;
	int		end;
	int		i;
	t_exp	*xx;

	ft_initexport(&xx, &head, &i, gc);
	ft_attribute(&xx, argv, ch);
	while (element->env[++i])
		ft_adder(&head, element->env[i], gc);
	if (!ft_dr(xx, head, element, gc) || !ft_opp(xx, head, element, gc))
		return (NULL);
	i = 0;
	while (xx->argv[++i])
	{
		if (!ft_exparser(xx->argv[i], element, gc))
			ft_adder(&head, xx->argv[i], gc);
		else
			ft_paramex(xx, element, gc);
	}
	adder = ft_filladder(xx, head, element, gc);
	if (xx->ch == 0)
		return (adder);
	end = xx->code;
	(gc_cleanup(gc), free_std(), exit(end));
}
