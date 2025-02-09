/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/20 18:20:48 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static void	ft_cit(size_t	*count, size_t *sw, int *sq, int *dq)
{
	*count = 0;
	*sw = 0;
	*sq = 0;
	*dq = 0;
}

size_t	ft_count(const char *str, const char sep)
{
	size_t	i;
	size_t	count;
	size_t	sw;
	int		sq;
	int		dq;

	i = 0;
	ft_cit(&count, &sw, &sq, &dq);
	while (str[i])
	{
		if (str[i] == '\'' && !dq)
			sq = !sq;
		if (str[i] == '\"' && !sq)
			dq = !dq;
		if (str[i] != sep && !sw)
		{
			count++;
			sw = 1;
		}
		if (str[i] == sep && !sq && !dq)
			sw = 0;
		i++;
	}
	return (count);
}

char	*ft_eachword(const char *str, int fidx, int end, t_gc *gc)
{
	char	*word;
	int		i;

	i = 0;
	word = gc_malloc((end - fidx + 1) * sizeof(char), gc);
	while (fidx < end)
	{
		word[i] = str[fidx];
		i++;
		fidx++;
	}
	word[i] = '\0';
	return (word);
}
//Verifier la pertinence avec l'arrivee de gc_cleaner
void	*ft_freesplit(char **tab, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		free(tab[i++]);
	free(tab);
	return (NULL);
}
