/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 13:35:30 by aisidore          #+#    #+#             */
/*   Updated: 2024/12/10 19:58:31 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// NB : 0.2 is considered as 0 for ints

#include "../minishell.h"

//AJOUTER FICHIER  .c DANS MAKEFILE et ft_itoa dans minishell.h

static size_t	ft_count(int nb)
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

char	*ft_itoa(int nb)
{
	char			*str;
	int				i;
	size_t			size;
	unsigned int	n;

	size = ft_count(nb);
	i = size - 1;
	str = ft_calloc(size + 1, sizeof(char));
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
