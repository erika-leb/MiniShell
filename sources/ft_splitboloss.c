/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitboloss.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:39:05 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/14 13:05:22 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	ft_count_(const char *str, const char sep)
{
	size_t	i;
	size_t	count;
	size_t	sameword;

	i = 0;
	count = 0;
	sameword = 0;
	while (str[i])
	{
		if (str[i] != sep && !sameword)
		{
			count++;
			sameword = 1;
		}
		if (str[i] == sep)
			sameword = 0;
		i++;
	}
	return (count);
}

static char	*ft_eachword_(const char *str, int firstindex, int end, t_gc *gc)
{
	char	*word;
	int		i;

	i = 0;
	word = gc_calloc(end - firstindex + 1, sizeof(char), gc);
	while (firstindex < end)
	{
		word[i] = str[firstindex];
		i++;
		firstindex++;
	}
	word[i] = '\0';
	return (word);
}

static void	ft_init_(size_t *i, size_t *j, int *firstindex)
{
	*i = 0;
	*j = 0;
	*firstindex = -1;
}

char	**ft_splitboloss(char const *str, char sep, t_gc *gc)
{
	char	**tab;
	size_t	i;
	size_t	j;
	int		firstindex;

	ft_init_(&i, &j, &firstindex);
	tab = gc_calloc (ft_count_(str, sep) + 1, sizeof(char *), gc);
	while (i <= ft_strlen(str))
	{
		if (str[i] != sep && firstindex < 0)
			firstindex = i;
		else if ((str[i] == sep || i == ft_strlen(str)) && firstindex >= 0)
		{
			tab[j] = ft_eachword_(str, firstindex, i, gc);
			firstindex = -1;
			j++;
		}
		i++;
	}
	return (tab);
}
