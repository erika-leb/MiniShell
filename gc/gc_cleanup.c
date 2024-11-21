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

/* Fonction utile pour libérer tous les mallocs d'un coup */

void gc_cleanup(t_gc *gc)
{
	t_gc_node	*current;
	t_gc_node	*next;

	current = gc->head;
	while (current != NULL)
	{
		next = current->next;
		free(current->ptr);
		free(current);
		current = next;
	}
	gc->head = NULL;
}