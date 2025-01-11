/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 14:39:05 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/11 19:00:03 by aisidore         ###   ########.fr       */
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

// char	**ft_splitter(char const *str, int sq, int dq)
// {
// 	char	**tab;
// 	size_t	i;
// 	size_t	j;
// 	int		fidx;

// 	ft_init(&i, &j, &fidx);
// 	tab = ft_calloc(2 + 1, sizeof(char *));
// 	//tab = ft_calloc(ft_count(str, '=') + 1, sizeof(char *));
// 	if (tab == NULL)
// 		return (NULL);
// 	while (i <= ft_strlen(str))
// 	{
// 		ft_modifquote(str, &sq, &dq, &i);
// 		if (str[i] != '=' && fidx < 0)
// 			fidx = i;
// 		else if (((str[i] == '=' && !sq && !dq)
// 				|| i == ft_strlen(str)) && fidx >= 0)
// 		{
// 			tab[j] = ft_eachword(str, fidx, i); 
// 			if (tab[j++] == NULL)
// 				return (ft_freesplit(tab, j - 1));
// 			fidx = -1;
// 		}
// 		i++;
// 	}
// 	return (tab);	
// }

//je cherche a faire en sorte que splitter s'arrete des qu'il observe un premier =
char	**ft_splitter(char const *str, int done)
{
	char	**tab;
	size_t	i;
	size_t	j;
	int		fidx;
	(void)done;

	ft_init(&i, &j, &fidx);
	tab = ft_calloc(2 + 1, sizeof(char *));
	if (tab == NULL)
		return (NULL);
	while (i <= ft_strlen(str))
	{
		if (str[i] != '=' && fidx < 0)
			fidx = i;
		else if ((str[i] == '=' || i == ft_strlen(str)) && fidx >= 0)
		{
			tab[j] = ft_eachword(str, fidx, i); 
			if (tab[j++] == NULL)
				return (ft_freesplit(tab, j - 1));
			fidx = -1;
			//done = 1;
		}
		i++;
	}
	return (tab);	
}

char	**ft_split(char const *str, int sq, int dq)
{
	char	**tab;
	size_t	i;
	size_t	j;
	int		fidx;

	ft_init(&i, &j, &fidx);
	tab = ft_calloc(ft_count(str, ' ') + 1, sizeof(char *));
	if (tab == NULL)
		return (NULL);
	while (i <= ft_strlen(str))
	{
		ft_modifquote(str, &sq, &dq, &i);
		if (str[i] != ' ' && fidx < 0)
			fidx = i;
		else if (((str[i] == ' ' && !sq && !dq)
				|| i == ft_strlen(str)) && fidx >= 0)
		{
			tab[j] = ft_eachword(str, fidx, i);
			if (tab[j++] == NULL)
				return (ft_freesplit(tab, j - 1));
			fidx = -1;
		}
		i++;
	}
	return (tab);
}
