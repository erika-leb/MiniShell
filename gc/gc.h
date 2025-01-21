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

#ifndef GC_H
#define GC_H

# include <stdio.h>//printf
# include <stdlib.h>//free, ...
# include <stdint.h>//SIZE_MAX
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
void	*gc_calloc(size_t n, size_t sizof, t_gc *gc);// a verifier si ca marche

#endif