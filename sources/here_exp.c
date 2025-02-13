/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/13 20:40:52 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static void	ft_expandnexth(char *result, int *k, char *tmp, char *envv)
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

static void	ft_expandh(char *result, int *k, t_element *elements, t_gc *gc)
{
	char	tmp[20000];
	char	*envv;
	int		i;
	size_t		len;

	i = 0;
	len = 0;
    envv = ft_getenvv(result, k, tmp, elements, gc);
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
	ft_expandnexth(result, k, tmp, envv);
	(*k)--;
}

char	*ft_hereifexpand(char *result, t_element *elements, t_gc *gc)
{
	int	k;

	k = 0;
	while (result[k])
	{
		if (result[k] == '$' && (result[k + 1] == '_'
			|| ft_isalnum(result[k + 1]) || result[k + 1] == '?'))
			ft_expandh(ft_erase(result, k), &k, elements, gc);
		k++;
	}
	result[k] = '\0';
	return (result);
}

char	*ft_hedgifexp(char *result, t_element *elements, t_gc *gc)
{
	int	k;

	k = 0;
	while (result[k] && result[k] != '\t')
	{
		if (result[k] == '$' && (result[k + 1] == '_'
			|| ft_isalnum(result[k + 1]) || result[k + 1] == '?'))
			ft_expandh(ft_erase(result, k), &k, elements, gc);
		k++;
	}
	result[k] = '\0';
	return (result);
}
