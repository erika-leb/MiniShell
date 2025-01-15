/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:21:11 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/15 10:44:20 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

// size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
// {
// 	int		i;
// 	size_t	s;

// 	i = 0;
// 	s = 0;
// 	while (src[s])
// 		s++;
// 	if (dstsize == 0)
// 		return (s);
// 	while (src[i] && dstsize > 1)
// 	{
// 		dst[i] = src[i];
// 		i++;
// 		dstsize--;
// 	}
// 	dst[i] = '\0';
// 	return (s);
// }

void    ft_buff_error(char *str, t_element *elements, t_gc *gc)
{
    int k;
    int i;
    int s1;
    int s2;
    
    (void) gc;
    k = 0;
    i = 0;
    if (!str)
        return ;
    s1 = ft_strlen(elements->error);
    s2 = ft_strlen(str);
    
    while(elements->error[k])
        k++;
    while(str[i])
        elements->error[k++] = str[i++];
}

void    ft_write_error(t_element *elements, t_gc *gc)
{
    int size;

    size = ft_strlen(elements->error);
    write(1, elements->error, size);
    gc_remove(gc, elements->error);
    elements->error = NULL;
}