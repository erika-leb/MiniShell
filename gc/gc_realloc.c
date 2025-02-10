/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:27:25 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/10 16:27:36 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

/* Fonction utile pour modifier un malloc identifié a partir de son pointeur */

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	char	*temp;
	char	*tp2;

	temp = (char *) dst;
	tp2 = (char *) src;
	if (dst == NULL && src == NULL)
		return (NULL);
	while (n)
	{
		*(temp++) = *(tp2++);
		n--;
	}
	return (dst);
}

size_t	gc_get_malloc_size(void *ptr, t_gc *gc)
{
	t_gc_node	*current;

	current = gc->head;
	while (current)
	{
		if (current->ptr == ptr)
			return (current->size);
		current = current->next;
	}
	return (0);
}

void	*gc_realloc(void *ptr, size_t new_size, t_gc *gc)
{
	void	*new_ptr;
	size_t	old_size;

	new_ptr = gc_malloc(new_size, gc);
	if (!new_ptr)
	{
		perror("allocation failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	if (ptr)
	{
		old_size = gc_get_malloc_size(ptr, gc);
		if (old_size > new_size)
			ft_memcpy(new_ptr, ptr, new_size);
		else
			ft_memcpy(new_ptr, ptr, old_size);
	}
	return (new_ptr);
}
