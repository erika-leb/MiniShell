/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/14 13:08:57 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_stopunset(t_element *element, t_gc *gc, int ch)
{
	if (ch == 0)
	{
		element->exit_status = ft_itoa(0, gc);
		return (1);
	}
	else
		(gc_cleanup(gc), free_std(), exit(0));
}

static int	ft_newenvv(t_env *head, t_element *element, t_gc *gc, int ch)
{
	char	**adder;

	adder = ft_ltoa(head, gc);
	gc_remove(gc, head);
	gc_remove(gc, element->env);
	element->env = adder;
	if (ft_stopunset(element, gc, ch))
		return (1);
	return (0);
}

void	ft_initwhile(t_env *head, t_env **current, t_env **previous)
{
	*current = head;
	*previous = NULL;
}

void	ft_gotonext(t_env **current, t_env **previous)
{
	*previous = *current;
	*current = (*current)->next;
}

void	ft_unset(t_element *element, char **argv, t_gc *gc, int ch)
{
	t_env	*head;
	t_env	*current;
	t_env	*previous;
	int		i;

	if (ft_initunset(&head, element->env, argv, gc))
	{
		if (ft_stopunset(element, gc, ch))
			return ;
	}
	i = 1;
	while (argv[i])
	{
		ft_initwhile(head, &current, &previous);
		while (current)
		{
			if (strcmp(current->name, argv[i]) == 0)
				ft_freeun(&current, &previous, &head, gc);
			else
				ft_gotonext(&current, &previous);
		}
		i++;
	}
	if (ft_newenvv(head, element, gc, ch))
		return ;
}
