/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/15 17:30:39 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_istok(char *av2)
{
	if (*av2 == '|' || *av2 == '<'
		|| *av2 == '>')
		return (1);
	return (0);
}

static void	ft_addtoken(char *result, const char *token, int *i, int *j)
{
	int	k;

	k = 0;
	while (token[k])
	{
		result[*j] = token[k];
		(*j)++;
		k++;
	}
	*i += k - 1;
}

static void	ft_addspace(char *result, char *av2, int *i, int *j)
{
	if (ft_istok(av2 + *i))
	{
		result[*j] = ' ';
		(*j)++;
	}
}

static void	ft_dotok(char *result, char *av2, int *i, int *j)
{
	int	k;
	int	sq;
	int	dq;

	k = -1;
	sq = 0;
	dq = 0;
	while (++k <= *i)
		ft_modifquote_(av2, &sq, &dq, &k);
	if (!sq && !dq)
		ft_addspace(result, av2, i, j);
	if (av2[*i] == '<' && av2[*i + 1] == '<')
		ft_addtoken(result, "<<", i, j);
	else if (av2[*i] == '>' && av2[*i + 1] == '>')
		ft_addtoken(result, ">>", i, j);
	else
	{
		result[*j] = av2[*i];
		(*j)++;
	}
	if (!sq && !dq)
		ft_addspace(result, av2, i, j);
}

char	*ft_tokenize(char *av2, t_gc *gc, t_element *elements)
{
	static char	result[70000];
	int			i;
	int			j;
	t_sqdq		*q;

	q = gc_malloc(sizeof(t_sqdq), gc);
	q->sq = 0;
	q->dq = 0;
	i = 0;
	j = 0;
	ft_deldollar(av2);
	while (av2[i])
	{
		if (ft_istok(av2 + i))
			ft_dotok(result, av2, &i, &j);
		else
			result[j++] = av2[i];
		i++;
	}
	result[j] = '\0';
	return (ft_ifexpand(result, q, elements, gc));
}
