/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_arr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:33:45 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/12 16:53:57 by aisidore         ###   ########.fr       */
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
static int		ft_countinside(char *tab_j, t_element *elements, t_gc *gc)
{
	char	*res;
	char	**restab;
	int		i;
	
	res = ft_filter(tab_j, elements, gc);
	restab = ft_splitboloss(res, ' ', gc);
	printf("lol : %s\n", res);
	i = 0;
	while (restab[i])
	{
		printf("tab [%d] = %s\n", i, restab[i]);
		i++;
	}
	printf("nb d'elements = %d\n", ft_arr_size(restab));
	if (ft_arr_size(restab) == 0 || ft_arr_size(restab) == 1)
		return (0);
	return (ft_arr_size(restab) - 1);
}

static void	ft_put_words(char **arr, char **tab, t_var *var, t_element *elements, t_gc *gc)
{
	int	s;
	// int	howmany;
	char	*res;
	char	**restab;
	int		m;

	if (ft_bst(tab[var->j]))
	{
		m = 0;
		res = ft_filter(tab[var->j], elements, gc);
		restab = ft_splitboloss(res, ' ', gc);
		while (restab[m])
		{
			arr[var->k] = restab[m];
			m++;
			(var->k)++;
		}
		// (var->k)--;
		return ;
	}
	// howmany = ft_countinside(tab[var->j], elements, gc);
	// if (howmany)
	// {
	// 	perror("pas sense etre la");
	// 	m = 0;
	// 	res = ft_filter(tab[var->j], elements, gc);
	// 	restab = ft_splitboloss(res, ' ', gc);
	// 	while (restab[m])
	// 	{
	// 		arr[var->k] = restab[m];
	// 		m++;
	// 		(var->k)++;
	// 	}
	// 	// (var->k)--;
	// 	return ;
	// }
	s = ft_strlen(tab[var->j]);
	if (ft_is_str(tab[var->j]) == true)
		arr[var->k] = ft_substr(tab[var->j], 1, s - 1, gc);
	else
	{
		perror("ici");
		arr[var->k] = gc_malloc(s + 1, gc);
		arr[var->k] = tab[var->j];
	}
	(var->k)++;
}
//Si j'ai un bloque $\tz\t$HOME$\tz\t ou un simple $\tz\t car me donne le nb de mots dans le block
static int		ft_bst_bis(char **tab, int last_i, int i, int arr_s, t_element *elements, t_gc *gc)
{
	int	j;
	int k;
	int	n;

	j = last_i + 1;
	if (i == arr_s - 1)
		i++;
	k = 0;
	while (j < i)
	{
		n = 0;
		while (tab[j][n] && tab[j][n + 1])
		{
			if (tab[j][n] == '$' && tab[j][n + 1] == '\t')
			{
				k += ft_countinside(tab[j], elements, gc);
				break;
			}
			n++;	
		}
		j++;
	}
	return (k);
}

int	nb_arg(char **tab, int i, int last_i, t_element *elements, t_gc *gc)
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
	k += ft_bst_bis(tab, last_i, i, arr_s, elements, gc);//k += 
	//Si chaine vide ou fait d'espaces alors on compte pas + d'elements.
	//Ici on ajoute le nb de mots que comptient $\tHOLA\t$\tHELLO\t...
	return (k);
}

char	**cmd_arr(char **tab, int i, int last_i, t_element *elements, t_gc *gc)
{
	int		s_arr;
	char	**arr;
	int		k;
	// t_var	*var;

	//(void) gc; ///ici on ajoute le cas \n command
	//var = gc_malloc(sizeof(var), gc);
	// var->j = i;
	// var->k = last_i;
	s_arr = nb_arg(tab, i, last_i, elements, gc);
	arr = gc_malloc(sizeof(char *) * (s_arr + 1), gc);
	ft_fill_arr(arr, tab, i, last_i, elements, gc);

	k = 0;
	while (arr[k])
	{
		printf("arr[%d] = %s\n", k, arr[k]);
		k++;
	}
	//ft_fill_arr(arr, tab, var, gc);
	return (arr);
}

void	ft_fill_arr(char **arr, char **tab, int i, int last_i, t_element *elements, t_gc *gc)
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
			ft_put_words(arr, tab, var, elements, gc);
			// (var->k)++;
		}
		//printf("arr[%d] = %s\n", var->k, tab[var->j]);
		(var->j)++;
	}
	arr[var->k] = NULL;
}

// void	ft_fill_arr(char **arr, char **tab, t_var *var, t_gc *gc)
// {
// 	int		s;
// 	t_var	*var_bis;

// 	s = ft_arr_size(tab);
// 	var_bis = ft_init_var(var->k, gc);
// 	if (var->j == s - 1)
// 		var->j++;
// 	while ((var_bis->j) < var->j)
// 	{
// 		if (ft_is_redir(tab[var_bis->j]) == true)
// 			(var_bis->j)++;
// 		else
// 		{
// 			ft_put_words(arr, tab, var_bis, gc);
// 			(var_bis->k)++;
// 		}
// 		(var_bis->j)++;
// 	}
// 	arr[var_bis->k] = NULL;
// }

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
