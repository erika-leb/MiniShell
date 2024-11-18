/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:39:05 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/17 16:28:56 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_init(size_t *i, size_t *j, int *fidx)
{
	*i = 0;
	*j = 0;
	*fidx = -1;
}

static void	ft_modifquote(char const *str, int *sq, int *dq, size_t *i)
{
	if (str[*i] == '\'' && !(*dq))
		*sq = !(*sq);
	if (str[*i] == '\"' && !(*sq))
		*dq = !(*dq);
}

static char	*ft_filltab(char const *str, int i, int fidx)
{
	if (str[i - 1] == '\'' || str[i - 1] == '\"')
		return (ft_eachword(str, fidx, i - 1));
	else
		return (ft_eachword(str, fidx, i));
}

static void	ft_start(int sq, int dq, size_t *i, int *fidx)
{
	if (sq || dq)
		*i = *i + 1;
	*fidx = *i;
}

char	**ft_split(char const *str, char sep, int sq, int dq)
{
	char	**tab;
	size_t	i;
	size_t	j;
	int		fidx;

	ft_init(&i, &j, &fidx);
	tab = ft_calloc(ft_count(str, sep) + 1, sizeof(char *));
	if (tab == NULL)
		return (NULL);
	while (i <= ft_strlen(str))
	{
		ft_modifquote(str, &sq, &dq, &i);
		if (str[i] != sep && fidx < 0)
			ft_start(sq, dq, &i, &fidx);
		else if (((str[i] == sep && !sq && !dq)
				|| i == ft_strlen(str)) && fidx >= 0)
		{
			tab[j] = ft_filltab(str, i, fidx);
			if (tab[j++] == NULL)
				return (ft_freesplit(tab, j - 1));
			fidx = -1;
		}
		i++;
	}
	return (tab);
}
