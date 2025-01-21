/*	************************************************************************** */
/*		                                                                    */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:31:45 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/21 13:46:29 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

/* Fonction utile pour libérer un malloc spécifique trouvé a l'aide de son pointeur */

void	gc_remove(t_gc *gc, void *ptr)
{
	t_gc_node	*current;
	t_gc_node	*prev;

	current = gc->head;
	prev = NULL;
	while (current)
	{
		if (current-> ptr == ptr)
		{
			if (prev)
				prev->next = current->next;
			else
				gc->head = current->next;
			free(current->ptr);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}