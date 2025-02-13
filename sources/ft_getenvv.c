/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenvv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/13 18:03:59 by aisidore         ###   ########.fr       */
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

static char	*ft_searchenv(char *tmp, t_element *elements, t_gc *gc)
{
	int		i;
	size_t		len;
	char	*to_search;
	char	*value;

	i = 0;
	to_search = ft_strjoin(tmp, "=", gc);
	len = ft_strlen(to_search);
	while (elements->env[i] && ft_strncmp(to_search, elements->env[i], len))
		i++;
	if (!elements->env[i])
		return (NULL);
	value = ft_cut(elements->env[i], '=', 1, gc);
	return (value);
}

char	*ft_getenvv(char *result, int *k, char *tmp, t_element *elements, t_gc *gc)
{
	int i;

    i = 0;
 	while (result[*k + i] && (result[*k + i] == '_' || ft_isalnum(result[*k + i])))
 	{
 		tmp[i] = result[*k + i];
 		i++;
 	}
	if (i == 0 && result[*k] == '?')
	{
		tmp[i] = '?';
		i++;
	}
    tmp[i] = '\0';
	if (!ft_strcmp(tmp, "?"))
		return (elements->exit_status);
	return (ft_searchenv(tmp, elements, gc));
}
