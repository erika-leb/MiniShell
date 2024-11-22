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
#include "minishell.h"

/* Fonctions utiles (gc_init et gc_malloc) pour créer la chaine répertoriant tous les malloc */

// Initialisation de la liste chainée

void gc_init(t_gc *gc) 
{
	gc->head = NULL;
}

void	gc_add(t_gc *gc, void *ptr, size_t size)
{
	t_gc_node	*new_node;

	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
	{
		perror("Error: allocation failed");
		gc_cleanup(gc);
		free(ptr);
		exit(EXIT_FAILURE);
	}
	new_node->ptr = ptr;
	new_node->size = size;
	new_node->next = gc->head;
	gc->head = new_node;
}

// Ajout d'un malloc a la chaine

void	*gc_malloc(size_t size, t_gc *gc)
{
	void	*ptr;

	ptr = malloc(size);
	// if (size == sizeof(t_cmd))
	// {
	// 	free(ptr);
	// 	ptr = NULL;
	// }
	if (!ptr)
	{
		perror("Error: allocation failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	gc_add(gc, ptr, size);
	return (ptr);
}
