/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:43 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/21 18:49:06 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	*ft_calloc(size_t n, size_t sizof)
{
	unsigned char	*buffer;
	size_t			i;

	if (n < 1 || sizof < 1 || n > SIZE_MAX / sizof)
		return (NULL);
	i = 0;
	buffer = malloc(n * sizof);
	if (buffer == NULL)
		return (NULL);
	while (i < n * sizof)
		buffer[i++] = (unsigned char)0;
	return (buffer);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

int	ft_arr_size(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}