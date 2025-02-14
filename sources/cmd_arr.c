/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:33:45 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/14 15:38:29 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_countinside(char *tab_j, t_element *elements, t_gc *gc)
{
	char	*res;
	char	**restab;

	res = ft_filter(tab_j, elements, gc);
	restab = ft_splitboloss(res, ' ', gc);
	if (ft_arr_size(restab) == 0 || ft_arr_size(restab) == 1)
		return (0);
	return (ft_arr_size(restab) - 1);
}

static void	ft_put_words(char **arr, t_arg *var, t_element *elements, t_gc *gc)
{
	int		s;
	char	*res;
	char	**restab;
	int		m;

	if (ft_bst((var->tab)[var->j]))
	{
		m = 0;
		res = ft_filter((var->tab)[var->j], elements, gc);
		restab = ft_splitboloss(res, ' ', gc);
		while (restab[m])
			arr[(var->k)++] = restab[m++];
		return ;
	}
	s = ft_strlen((var->tab)[var->j]);
	if (ft_is_str((var->tab)[var->j]) == true)
		arr[var->k] = ft_substr((var->tab)[var->j], 1, s - 1, gc);
	else
	{
		arr[var->k] = gc_malloc(s + 1, gc);
		arr[var->k] = (var->tab)[var->j];
	}
	(var->k)++;
}

static int	ft_bst_bis(t_arg *arg, int arr_s, t_element *elements, t_gc *gc)
{
	int	j;
	int	k;
	int	n;
	int	m;

	m = arg->i;
	j = (arg->last_i) + 1;
	if (arg->i == arr_s - 1)
		m++;
	k = 0;
	while (j < m)
	{
		n = 0;
		while ((arg->tab)[j][n] && (arg->tab)[j][n + 1])
		{
			if ((arg->tab)[j][n] == '$' && (arg->tab)[j][n + 1] == '\t')
			{
				k += ft_countinside((arg->tab)[j], elements, gc);
				break ;
			}
			n++;
		}
		j++;
	}
	return (k);
}

int	nb_arg(t_arg *arg, t_element *elements, t_gc *gc)
{
	int	j;
	int	k;
	int	arr_s;
	int	l;

	l = arg->i;
	arr_s = ft_arr_size(arg->tab);
	j = (arg->last_i) + 1;
	if (l == arr_s - 1)
		l++;
	k = 0;
	while (j < l)
	{
		if (ft_is_redir(arg->tab[j]) == true)
			j++;
		else
			k++;
		j++;
	}
	k += ft_bst_bis(arg, arr_s, elements, gc);
	return (k);
}

void	ft_fill_arr(char **arr, t_arg *arg, t_element *elements, t_gc *gc)
{
	int		s;
	int		m;

	s = ft_arr_size(arg->tab);
	arg->j = arg->last_i + 1;
	m = arg->i;
	arg->k = 0;
	if (arg->i == s - 1)
		m = m + 1;
	while ((arg->j) < m)
	{
		if (ft_is_redir((arg->tab)[arg->j]) == true)
			(arg->j)++;
		else
			ft_put_words(arr, arg, elements, gc);
		(arg->j)++;
	}
	arr[arg->k] = NULL;
}

// void	ft_fill_arr(char **arr, t_arg *arg, t_element *elements, t_gc *gc)
// {
// 	int		s;

// 	s = ft_arr_size(arg->tab);
// 	arg->j = arg->last_i + 1;
// 	arg->k = 0;
// 	if (arg->i == s - 1)
// 		arg->i = arg->i + 1;
// 	while ((arg->j) < arg->i)
// 	{
// 		if (ft_is_redir((arg->tab)[arg->j]) == true)
// 			(arg->j)++;
// 		else
// 			ft_put_words(arr, arg, elements, gc);
// 		(arg->j)++;
// 	}
// 	arr[arg->k] = NULL;
// }
