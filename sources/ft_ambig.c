/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ambig.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/13 15:13:34 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static int	ft_moredoll(char *str, int i, int sq, int dq)
{
	if (str[0] && !str[1])
		return (1);
	while (str[i])
	{
		ft_modifquote_(str, &sq, &dq, &i);
		if (str[i] == '$' && !sq && !dq)
			return (1);
		i++;
	}
	return (0);
}

static void	ft_ibslash(char *result_k, char *name, int *k)
{
	int	m;

	m = 0;
	(*k)++;
	ft_insert(result_k, 1, '\n');
	while (name[m])
	{
		m++;
		(*k)++;
	}
}

static void	ft_initambig(char *result_k, char *name, int *m)
{
	*m = -1;
	while (result_k[++(*m)] && result_k[*m] != ' ')
		name[*m] = result_k[*m];
	name[*m] = '\0';
	*m = 0;
}

void	ft_ambig(char *result_k, int *k, t_element *elements, t_gc *gc)
{
	char	*envv;
	char	tmp[20000];
	char	name[20000];
	int		m;

	if (*result_k != '$')
		return ;
	envv = NULL;
	ft_initambig(result_k, name, &m);
	while (name[m])
	{
		if (name[m] == '$' && (*(name + m + 1) == '_'
			|| ft_isalnum(*(name + m + 1)) || *(name + m + 1) == '?'))
			envv = ft_getenvv(name + 1, &m, tmp, elements, gc);
		while (name[m] != '$' && (*(name + m) == '_'
				|| ft_isalnum(*(name + m)) || *(name + m) == '?'))
			m++;
		if (envv || !ft_moredoll(name + m, 0, 0, 0))
			break ;
		m++;
	}
	if (!envv && !name[m])
		ft_ibslash(result_k, name, k);
}
