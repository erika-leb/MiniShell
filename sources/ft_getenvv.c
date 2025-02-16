/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenvv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/16 21:32:22 by ele-borg         ###   ########.fr       */
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
	size_t	len;
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

char	*ft_getenvv(t_forenvv *ev, char *tmp, int *k, t_gc *gc)
{
	int	i;

	i = 0;
	while (ev->result[*k + i] && (ev->result[*k + i] == '_'
			|| ft_isalnum(ev->result[*k + i])))
	{
		tmp[i] = ev->result[*k + i];
		i++;
	}
	if (i == 0 && ev->result[*k] == '?')
	{
		tmp[i] = '?';
		i++;
	}
	tmp[i] = '\0';
	if (!ft_strcmp(tmp, "?"))
		return (ev->elements->exit_status);
	return (ft_searchenv(tmp, ev->elements, gc));
}

void	ft_spacequotes(char	*result_k, t_element *elements, t_gc *gc)
{
	char		tmp[SIZE_MID];
	char		*envv;
	int			i;
	t_forenvv	*ev;

	i = 1;
	if (*result_k != '$')
		return ;
	ev = gc_malloc(sizeof(t_forenvv), gc);
	ev->result = ft_strdup_(result_k, gc);
	ev->elements = elements;
	envv = ft_getenvv(ev, tmp, &i, gc);
	i = 0;
	while (envv && envv[i])
	{
		if (envv[i] == ' ' || envv[i] == '\'' || envv[i] == '\"')
		{
			ft_insert(result_k, 1, '\t');
			ft_insert(result_k, ft_strlen(tmp) + 2, '\t');
			return ;
		}
		i++;
	}
}
