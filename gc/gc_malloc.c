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
	//new_node = ft_calloc(1, sizeof(t_gc_node));
	if (!new_node)
	{
		perror("allocation failed");
		gc_cleanup(gc);
		free(ptr);
		exit(EXIT_FAILURE);
	}
	new_node->ptr = ptr;
	new_node->size = size;
	new_node->next = gc->head;
	gc->head = new_node;
	//printf("Added pointer %p to GC (size: %zu)\n", ptr, size);
}

// Ajout d'un malloc a la chaine

void	*gc_malloc(size_t size, t_gc *gc)
{
	void	*ptr;

	//(void) size;
	//size_t huge_size = (size_t)1 << 40;
	ptr = malloc(size);
	//ptr = ft_calloc(size);
	// printf("allocation de %zu size\n", size);
	// if (size == sizeof(t_cmd))
	// {
	// 	free(ptr);
	// 	ptr = NULL;
	// }
	if (!ptr)
	{
		perror("allocation failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	gc_add(gc, ptr, size);
	return (ptr);
}

void	*gc_calloc(size_t n, size_t sizof, t_gc *gc) // a verifier si ca marche
{
	void	*ptr;

	ptr = ft_calloc(n, sizof);
	//ptr = ft_calloc(size);
	// printf("allocation de %zu size\n", size);
	// if (size == sizeof(t_cmd))
	// {
	// 	free(ptr);
	// 	ptr = NULL;
	// }
	if (!ptr)
	{
		perror("allocation failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	gc_add(gc, ptr, n * sizof);
	return (ptr);
}
