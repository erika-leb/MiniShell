/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ambig.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/14 18:20:56 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static int	ft_initambig(char *result_k, char *name, int *m)
{
	if (*result_k != '$')
		return (1);
	*m = -1;
	while (result_k[++(*m)] && result_k[*m] != ' ')
		name[*m] = result_k[*m];
	name[*m] = '\0';
	*m = -1;
	return (0);
}

static t_forenvv	*ft_alloc_ev(char *name, t_element *elements, t_gc *gc)
{
	t_forenvv	*ev;

	ev = gc_malloc(sizeof(t_forenvv), gc);
	ft_initev(ev, name + 1, elements, gc);
	return (ev);
}

void	ft_ambig(char *result_k, int *k, t_element *elements, t_gc *gc)
{
	char		*envv;
	char		tmp[20000];
	char		nm[20000];
	int			m;
	t_forenvv	*ev;

	envv = NULL;
	if (ft_initambig(result_k, nm, &m) == 1)
		return ;
	while (nm[++m])
	{
		if (nm[m] == '$' && (*(nm + m + 1) == '_' || ft_isalnum(*(nm + m + 1))
				|| *(nm + m + 1) == '?'))
		{
			ev = ft_alloc_ev(nm, elements, gc);
			envv = ft_getenvv(ev, tmp, &m, gc);
		}
		while (nm[m] != '$' && (*(nm + m) == '_' || ft_isalnum(*(nm + m))
				|| *(nm + m) == '?'))
			m++;
		if (envv || !ft_moredoll(nm + m, 0, 0, 0))
			break ;
	}
	if (!envv && !nm[m])
		ft_ibslash(result_k, nm, k);
}
