/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 16:28:54 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/10 16:29:47 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GC_H
# define GC_H

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_gc_node
{
	void				*ptr;
	size_t				size;
	struct s_gc_node	*next;
}	t_gc_node;

typedef struct s_gc
{
	t_gc_node	*head;
}	t_gc;

void	gc_init(t_gc *gc);
void	gc_add(t_gc *gc, void *ptr, size_t size);
void	*gc_malloc(size_t size, t_gc *gc);
void	gc_cleanup(t_gc *gc);
void	gc_remove(t_gc *gc, void *ptr);
size_t	gc_get_malloc_size(void *ptr, t_gc *gc);
void	*gc_realloc(void *ptr, size_t new_size, t_gc *gc);
void	*gc_calloc(size_t n, size_t sizof, t_gc *gc);

#endif
