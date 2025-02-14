/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:35:11 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/14 13:06:52 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_initunset(t_env **ptr_head, char **env, char **argv, t_gc *gc)
{
	int	i;

	if (!argv[1])
		return (1);
	if (argv[1][0] == '-')
	{
		write(2, "minishell: unset: No option(s) allowed\n", 41);
		return (1);
	}
	*ptr_head = NULL;
	i = -1;
	while (env[++i])
		ft_adder(ptr_head, env[i], gc);
	return (0);
}

void	ft_freeun(t_env **current, t_env **previous, t_env **head, t_gc *gc)
{
	t_env	*temp;

	temp = *current;
	if (*previous)
		(*previous)->next = (*current)->next;
	else
		*head = (*current)->next;
	*current = (*current)->next;
	gc_remove(gc, temp->name);
	gc_remove(gc, temp->key);
	gc_remove(gc, temp);
}
