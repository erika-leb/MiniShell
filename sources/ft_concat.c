/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_concat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/20 18:20:20 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static char	*ft_retmerge(char *result_i, int is_concat)
{
	//Erika : '\n<' veut dire qu'on prend la valeur litterale de <.
	// '<' veut dire qu'on prend '<' tout simplement.
	if (is_concat && ft_istok_(result_i))
	{
		//ft_insert(result_i, 0, '\n');
		//Taille memoire trop petitede1 byte. Je devrais remalloc la chaine '<'
		//pour passer de 3 bytes a 4 bytes ('\n<') mais flemme.
		// ft_insert(ft_insert(result_i, 0, '\''), ft_strlen(result_i), '\'');

		//AUTRE SOLUTION :
		ft_insert(result_i, 0, '\n');

	}
	return (result_i);
}

static void	ft_initconcat(int *to_erase, int k, int *is_concat)
{
	*to_erase = k;
	*is_concat = 0;
}

//SOUCI : si je créé une var d'env et que je lui attribue la valeur 'hello' alors ft_concat va modifier a tord
//cette variable. C'est pourquoi il vaut mieux expand les var d'env CONTENANT DES QUOTES apres ft_concat.
char	*ft_concat(char *result_i, int k, int sq, int dq)
{
	int	to_erase;
	int	is_concat;

	while (result_i[++k] && !(result_i[k] == '\'' || result_i[k] == '\"'))
	if (!result_i[k + 1])
		return (result_i);
	ft_initconcat(&to_erase, k, &is_concat);
	ft_modifquote_(result_i, &sq, &dq, &k);
	while (result_i[++k])
	{
		if (result_i[k] == result_i[to_erase])
		{
			is_concat = 1;
			ft_erase(result_i, to_erase);
			k--;
			ft_erase(result_i, k);
			k--;
			while (result_i[++k] && !(result_i[k] == '\''
					|| result_i[k] == '\"'))
			if (!result_i[k])
				return (result_i);
			to_erase = k;
		}
	}
	return (ft_retmerge(result_i, is_concat));
}
