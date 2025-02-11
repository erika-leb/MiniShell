/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_exp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/11 20:06:00 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

//adri42
//Si la taille de key < a la taille de name alors invalid read
static void	ft_expand(char *result, int *k, t_element *elements, t_gc *gc)
{
	char	tmp[20000];
	char	*envv;
	int		i;
	size_t		len;

	i = 0;
	len = 0;
    envv = ft_getenvv(result, k, tmp, elements, gc);//si y'a des quotes alors j'expand pas et envv prend la valeur de $\n\t ...
    if (!envv)
		return (ft_erase_substr(result, k, tmp));
	//Si envv contient des quotes (qu'il soit full ou pas on s'en fiche) alors on n'expand pas et on
	//remet le dollar + /n/t. Je n'oublie pas d'ecraser tmp comme fait juste en dessous.
	//ft_hedgecase va inserer $ + /n/t et incrementer k de ft_strlen(tmp) + 3. Ensuite on se sert d'une fonction
	//qui ressemble a ft_fatalerror pour expand, et resize un new_array avec une chaine de la bonne taille et eventuellement
	//split (sep = ' ') si y'avait des espaces en + des quotes.
	// if (ft_spacequotes(envv) == 2)
		// return (ft_hedgecase(result, k, tmp));

	//On partait du principe que la taille de key etait superieur a name. Donc d'abord on ajoutait dans result par exemple 6 caracteres
	//si name est de taille 6. Puis la seconde boucle while insere les elements manquant en deplacant tous les caracters d'un cran
	//vers la droite. Maintenant on remplit ecrase les valeurs de tmp
	while (tmp[i] && envv[i])
 	{
		result[*k] = envv[i];
		(*k)++;
		i++;
 	}
	//Ensuite on insere les valeurs de envv restantes.
	while (envv[i])
	{
		ft_insert(result, (*k), envv[i]);
		(*k)++;
		i++;
	}
	//arrive ici on a ecrase les caracteres de tmp par ceux de env et on a inserer les caracteres restants de env.
	//Il faut maintenant supprimer l'excedent de caracteres de tmp qu'il pourrait rester
	if (ft_strlen(tmp) > ft_strlen(envv))
	{
		len = ft_strlen(tmp) - ft_strlen(envv);
		while (len)
		{
			ft_erase(result, *k);
			len--;
		}
	}
	(*k)--;
}

//adri42
char	*ft_hereifexpand(char *result, t_element *elements, t_gc *gc)
{
	int	k;

	k = 0;
	while (result[k])
	{
		if (result[k] == '$' && (result[k + 1] == '_'
			|| ft_isalnum(result[k + 1]) || result[k + 1] == '?'))
			ft_expand(ft_erase(result, k), &k, elements, gc);//k n'est pas incremente, j'envoie qu'une copie.
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
			ft_expand(ft_erase(result, k), &k, elements, gc);//k n'est pas incremente, j'envoie qu'une copie.
		k++;
	}
	result[k] = '\0';
	return (result);
}
