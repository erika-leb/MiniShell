/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ifexpand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/16 21:32:08 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_expandnext(char *result, int *k, char *tmp, char *envv)
{
	size_t	len;

	len = 0;
	if (ft_strlen(tmp) > ft_strlen(envv))
	{
		len = ft_strlen(tmp) - ft_strlen(envv);
		while (len)
		{
			ft_erase(result, *k);
			len--;
		}
	}
}

static void	ft_expand(char *result, int *k, t_element *elements, t_gc *gc)
{
	char		tmp[SIZE_MID];
	char		*envv;
	int			i;
	t_forenvv	*ev;

	ev = gc_malloc(sizeof(t_forenvv), gc);
	ft_initev(ev, result, elements, gc);
	i = 0;
	envv = ft_getenvv(ev, tmp, k, gc);
	if (!envv)
		return (ft_erase_substr(result, k, tmp));
	while (tmp[i] && envv[i])
	{
		result[*k] = envv[i];
		(*k)++;
		i++;
	}
	while (envv[i])
	{
		ft_insert(result, (*k), envv[i]);
		(*k)++;
		i++;
	}
	ft_expandnext(result, k, tmp, envv);
	(*k)--;
}

static void	ft_delim(char *result, int *k, int sq, int dq)
{
	*k += 2;
	while (result[*k] == ' ')
		(*k)++;
	ft_modifquote_(result, &sq, &dq, k);
	if (sq)
	{
		(*k)++;
		while (result[*k] && !(result[*k] == '\''
				&& result[*k + 1] == ' '))
			(*k)++;
	}
	else if (dq)
	{
		(*k)++;
		while (result[*k] && !(result[*k] == '\"'
				&& result[*k + 1] == ' '))
			(*k)++;
	}
	else
	{
		while (result[*k] && result[*k] != ' ')
			(*k)++;
	}
}

static void	ft_incrk(char *result, int *k, t_sqdq *q)
{
	(*k)++;
	if (!ft_strncmp(result + *k, "> ", 2))
		(*k)++;
	while (result[*k] == ' ')
		(*k)++;
	ft_modifquote_(result, &q->sq, &q->dq, k);
}

char	*ft_ifexpand(char *res, t_sqdq *q, t_element *elements, t_gc *gc)
{
	int	k;

	k = 0;
	while (res[k])
	{
		ft_modifquote_(res, &q->sq, &q->dq, &k);
		if (!q->sq && !q->dq && !ft_strncmp(res + k, "<< ", 3))
			ft_delim(res, &k, 0, 0);
		if (!q->sq && !q->dq && (!ft_strncmp(res + k, ">> ", 3)
				|| !ft_strncmp(res + k, "> ", 2)
				|| !ft_strncmp(res + k, "< ", 2)))
		{
			ft_incrk(res, &k, q);
			if (!q->sq && !q->dq)
				ft_ambig(res + k, &k, elements, gc);
		}
		if (!q->sq && !q->dq)
			ft_spacequotes(res + k, elements, gc);
		if (res[k] == '$' && !q->sq && (res[k + 1] == '_'
				|| ft_isalnum(res[k + 1]) || res[k + 1] == '?'))
			ft_expand(ft_erase(res, k), &k, elements, gc);
		k++;
	}
	res[k] = '\0';
	return (res);
}
