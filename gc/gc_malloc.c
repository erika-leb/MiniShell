/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:25:03 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/10 17:00:39 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"
#include "minishell.h"

/* Fonctions utiles pour créer la chaine répertoriant tous les malloc */

void	gc_init(t_gc *gc)
{
	gc->head = NULL;
}

void	gc_add(t_gc *gc, void *ptr, size_t size)
{
	t_gc_node	*new_node;

	new_node = malloc(sizeof(t_gc_node));
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
}

void	*gc_malloc(size_t size, t_gc *gc)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
	{
		perror("allocation failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	gc_add(gc, ptr, size);
	return (ptr);
}

void	*gc_calloc(size_t n, size_t sizof, t_gc *gc)
{
	void	*ptr;

	ptr = ft_calloc(n, sizof);
	if (!ptr)
	{
		perror("allocation failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	gc_add(gc, ptr, n * sizof);
	return (ptr);
}
