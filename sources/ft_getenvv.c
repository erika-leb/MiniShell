/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenvv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/06 14:52:51 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static size_t	ft_countt(int nb)
{
	size_t	size;

	size = 0;
	if (nb == 0)
		size++;
	if (nb == INT_MIN)
		return (11);
	if (nb < 0)
	{
		nb = -nb;
		size++;
	}
	while (nb > 0)
	{
		nb = nb / 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int nb, t_gc *gc)
{
	char			*str;
	int				i;
	size_t			size;
	unsigned int	n;

	size = ft_countt(nb);
	i = size - 1;
	str = gc_calloc(size + 1, sizeof(char), gc);
	//str = ft_calloc(size + 1, sizeof(char));//a supprimer dans le garbage collector ?
	if (str == NULL)
		return (NULL);
	if (nb == 0)
		str[0] = '0';
	if (nb < 0)
	{
		str[0] = '-';
		n = -nb;
	}
	else
		n = nb;
	while (n > 0)
	{
		str[i--] = n % 10 + '0';
		n = n / 10;
	}
	return (str);
}

//adri42
static char	*ft_searchenv(char *tmp, t_element *elements, t_gc *gc)
{
	int		i;
	size_t		len;
	char	*to_search;
	char	*value;

	i = 0;
	//Si tmp ="HOME" alors to_search ="HOME="
	to_search = ft_strjoin(tmp, "=", gc);
	len = ft_strlen(to_search);
	while (elements->env[i] && ft_strncmp(to_search, elements->env[i], len))
		i++;
	if (!elements->env[i])
		return (NULL);//Si la var d'env n'existe pas on renvoie NULL comme getenv
	//Sinon on recupere la valeur de la var d'env, a savoir tout ce qui se situe apres le 1er =
	value = ft_cut(elements->env[i], '=', 1, gc);
	return (value);
}

//adri42
char	*ft_getenvv(char *result, int *k, char *tmp, t_element *elements, t_gc *gc)
{
	int i;

    i = 0;
 	while (result[*k + i] && (result[*k + i] == '_' || ft_isalnum(result[*k + i])))
 	{
 		tmp[i] = result[*k + i];
 		i++;
 	}
	//si i = 0 ici c'est peut etre qu'il y avait un ?, dans ce cas on regarde si c'est le cas et si oui
	//alors on incorpore 1 seul dans tmp avant de mettre \0. Ainsi si on ecrit $??? ca donne qqchose comme 0?? (comme dans bash)
	if (i == 0 && result[*k] == '?')
	{
		tmp[i] = '?';
		i++;
	}
    tmp[i] = '\0';
	//printf("coucou\n");
	if (!ft_strcmp(tmp, "?"))
		return (elements->exit_status);//il faudrait enregistrer le errno au debut du prg (dans une structure avec d'autres trucs utiles)
	//pour que chaque enfant puisse modifier sa valeur. Puis y faire appel dans getenvv pour choper la derniere valeur prise
    // return (getenv(tmp));
	return (ft_searchenv(tmp, elements, gc));
}
