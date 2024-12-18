/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:39:05 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/27 16:38:48 by ele-borg         ###   ########.fr       */
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

//Ces 2 fonctions permettent de zapper le quote de debut et de fin de chaque token
// static char	*ft_filltab(char const *str, int i, int fidx)
// {
// 	//if (str[i - 1] == '\'' || str[i - 1] == '\"')
// 	//	return (ft_eachword(str, fidx, i - 1));
// 	//else
// 		return (ft_eachword(str, fidx, i));
// }

// static void	ft_start(int sq, int dq, size_t *i, int *fidx)
// {
// 	if (sq || dq)
// 		(*i)++;
// 	*fidx = *i;
// }

char	**ft_split(char const *str, int sq, int dq, t_gc *gc)
{
	char	**tab;
	size_t	i;
	size_t	j;
	int		fidx;

	ft_init(&i, &j, &fidx);
	//tab = ft_calloc(ft_count(str, ' ') + 1, sizeof(char *));
	tab = gc_malloc((ft_count(str, ' ') + 1) * sizeof(char *), gc);
	// if (tab == NULL)
	// 	return (NULL);
	while (i <= ft_strlen(str))
	{
		ft_modifquote(str, &sq, &dq, &i);
		if (str[i] != ' ' && fidx < 0)
			fidx = i;
		else if (((str[i] == ' ' && !sq && !dq)
				|| i == ft_strlen(str)) && fidx >= 0)
		{
			tab[j] = ft_eachword(str, fidx, i, gc);
			j++;
			// if (tab[j++] == NULL)
			// 	return (ft_freesplit(tab, j - 1));
			fidx = -1;
		}
		i++;
	}
	tab[j] = NULL;
	return (tab);
}
