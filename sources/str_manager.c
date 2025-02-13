
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/20 18:06:31 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

char *ft_insert(char *result, int k, char c)
{
	int len;
	int i;

	len = ft_strlen(result);
	i = len;
	while (i >= k)
	{
		result[i + 1] = result[i];
		i--;
	}
	result[k] = c;
	return (result);
}

char *ft_erase(char *result, int tmp_k)
{
	while (result[tmp_k])
	{
		result[tmp_k] = result[tmp_k + 1];
		tmp_k++;
	}
	return (result);
}

void ft_erase_substr(char *result, int *k, char *tmp)
{
    int i;

    i = 0;
	while(tmp[i++])
		ft_erase(result, *k);
	(*k)--;
	return ;
}

void	ft_modifquote_(char const *str, int *sq, int *dq, int *i)
{
	if (str[*i] == '\'' && !(*dq))
		*sq = !(*sq);
	if (str[*i] == '\"' && !(*sq))
		*dq = !(*dq);
}
