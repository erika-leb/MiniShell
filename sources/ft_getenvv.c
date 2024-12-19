/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:35:30 by aisidore          #+#    #+#             */
/*   Updated: 2024/12/11 15:28:19 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static char	*ft_itoa(int nb)
{
	char			*str;
	int				i;
	size_t			size;
	unsigned int	n;

	size = ft_countt(nb);
	i = size - 1;
	str = ft_calloc(size + 1, sizeof(char));//a supprimer dans le garbage collector ?
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

// static char	*ft_geterrcode(void)
// {
// 	// int		err;
// 	// char	*str_err;

// 	// err = errno;
// 	// str_err = ft_itoa(err);
// 	// return (str_err);

// 	//ft_itoa présent dans Makefile, .h, mais inutilisé pour le moment
// 	//Il se peut que ft_itoa soit useless. Il faut juste récupérer le code
// 	//errno. On peut potentiellement le faire avec une variable globale.
// 	return("0");
// }

char	*ft_getenvv(char *result, int *k, char *tmp)
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
	if (!ft_strcmp(tmp, "?"))
		return (ft_itoa(0));
    return (getenv(tmp));
}