/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:33:45 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/11 17:03:42 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_var	*ft_init_var(int last_i, t_gc *gc)
{
	t_var	*var;

	var = gc_malloc(sizeof(t_var), gc);
	var->j = last_i + 1;
	var->k = 0;
	return (var);
}

static void	ft_put_words(char **arr, char **tab, t_var *var, t_gc *gc)
{
	int	s;

	s = ft_strlen(tab[var->j]);
	if (ft_is_str(tab[var->j]) == true)
		arr[var->k] = ft_substr(tab[var->j], 1, s - 1, gc);
	else
	{
		arr[var->k] = gc_malloc(s + 1, gc);
		arr[var->k] = tab[var->j];
	}
}

int	nb_arg(char **tab, int i, int last_i)
{
	int	j;
	int	k;
	int	arr_s;

	arr_s = ft_arr_size(tab);
	j = last_i + 1;
	if (i == arr_s - 1)
		i++;
	k = 0;
	while (j < i)
	{
		if (ft_is_redir(tab[j]) == true)
			j++;
		else
			k++;
		j++;
	}
	return (k);
}

char	**cmd_arr(char **tab, int i, int last_i, t_gc *gc)
{
	int		s_arr;
	char	**arr;
	t_var	*var;

	(void) gc; ///ici on ajoute le cas \n command
	var = gc_malloc(sizeof(var), gc);
	var->j = i;
	var->k = last_i;
	s_arr = nb_arg(tab, i, last_i);
	arr = gc_malloc(sizeof(char *) * (s_arr + 1), gc);
	//ft_fill_arr(arr, tab, i, last_i, gc);
	ft_fill_arr(arr, tab, var, gc);
	return (arr);
}

void	ft_fill_arr(char **arr, char **tab, t_var *var, t_gc *gc)
{
	int		s;
	t_var	*var_bis;

	s = ft_arr_size(tab);
	var_bis = ft_init_var(var->k, gc);
	if (var->j == s - 1)
		var->j++;
	while ((var_bis->j) < var->j)
	{
		if (ft_is_redir(tab[var_bis->j]) == true)
			(var_bis->j)++;
		else
		{
			ft_put_words(arr, tab, var_bis, gc);
			(var_bis->k)++;
		}
		(var_bis->j)++;
	}
	arr[var_bis->k] = NULL;
}

// void	ft_fill_arr(char **arr, char **tab, int i, int last_i, t_gc *gc)
// {
// 	int		s;
// 	t_var	*var;

// 	s = ft_arr_size(tab);
// 	var = ft_init_var(last_i, gc);
// 	if (i == s - 1)
// 		i++;
// 	while ((var->j) < i)
// 	{
// 		if (ft_is_redir(tab[var->j]) == true)
// 			(var->j)++;
// 		else
// 		{
// 			ft_put_words(arr, tab, var, gc);
// 			(var->k)++;
// 		}
// 		(var->j)++;
// 	}
// 	arr[var->k] = NULL;
// }
