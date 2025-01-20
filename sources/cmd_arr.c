/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:33:45 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/20 16:58:19 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static t_var	*ft_init_var(int last_i, t_gc *gc)
{
	t_var *var;

	var = gc_malloc(sizeof(t_var), gc);
	var->j = last_i + 1;
	var->k = 0;
	return (var);
}

static void ft_put_words(char **arr, char **tab, t_var *var, t_gc *gc)
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

	(void) gc;
	s_arr = nb_arg(tab, i, last_i);
	arr = gc_malloc(sizeof(char *) * (s_arr + 1), gc);
	ft_fill_arr(arr, tab, i, last_i, gc);
	return (arr);
}

void	ft_fill_arr(char **arr, char **tab, int i, int last_i, t_gc *gc)
{
	int		s;
	t_var	*var;

	s = ft_arr_size(tab);
	var = ft_init_var(last_i, gc);
	if (i == s - 1)
		i++;
	while ((var->j) < i)
	{
		if (ft_is_redir(tab[var->j]) == true)
			(var->j)++;
		else
		{
			ft_put_words(arr, tab, var, gc);
			(var->k)++;
		}
		//printf("arr[%d] = %s\n", var->k, tab[var->j]);
		(var->j)++;
	}
	arr[var->k] = NULL;
}

