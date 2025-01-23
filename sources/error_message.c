/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 17:21:11 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/23 17:04:15 by ele-borg         ###   ########.fr       */
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

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	sd;
	size_t	sc;
	size_t	i;
	size_t	k;

	sd = ft_strlen(dst);
	sc = ft_strlen(src);
	i = 0;
	k = 0;
	while (dst[i])
		i++;
	if (sd < dstsize && dstsize > 0)
	{
		while (src[k] && dstsize - sd > 1)
		{
			dst[i] = src[k];
			i++;
			k++;
			dstsize--;
		}
		dst[i] = '\0';
	}
	if (dstsize < sd)
		sd = dstsize;
	return (sd + sc);
}

int	ft_strlcpy(char *dst, const char *src, int dstsize)
{
	int		i;
	int	s;

	i = 0;
	s = 0;
	while (src[s])
		s++;
	if (dstsize == 0)
		return (s);
	while (src[i] && dstsize > 1)
	{
		dst[i] = src[i];
		i++;
		dstsize--;
	}
	dst[i] = '\0';
	return (s);
}

void	ft_buff_error(char *str, t_element *elements, t_gc *gc)
{
	int		s1;
	int		s2;
	char	*s;
	int		i;

	i = 0;
	s1 = ft_strlen(elements->error);
	s2 = ft_strlen(str);
	if (!elements->error)
	{
		elements->error = gc_malloc(sizeof(char) * (s2 + 1), gc);
		ft_strlcpy(elements->error, str, s2 + 1);
	}
	else
	{
		s = gc_malloc(sizeof(char) * (s2 + s1 + 1), gc);
		ft_strlcpy(s, elements->error, s2 + s1 + 1);
		// while (elements->error[i])
		// {
		// 	s[i] = elements->error[i];
		// 	i++;
		// }
		//ft_putstr_fd(s, 1);
		ft_strlcat(s, str, s1 + s2 + 1);
		gc_remove(gc, elements->error);
		elements->error = s;
	}
}

// void    ft_buff_error(char *str, t_element *elements, t_gc *gc)
// {
//     int k;
//     int i;
//     int s1;
//     int s2;

//     (void) gc;
//     k = 0;
//     i = 0;
//     if (!str)
//         return ;
//     s1 = ft_strlen(elements->error);
//     s2 = ft_strlen(str);

//     while(elements->error[k])
//         k++;
//     while(str[i])
//         elements->error[k++] = str[i++];
// }

void	ft_write_error(t_element *elements, t_gc *gc)
{
	int size;

	if (!elements->env)
		return ;
	size = ft_strlen(elements->error);
	write(2, elements->error, size);
	gc_remove(gc, elements->error);
	elements->error = NULL;
}
