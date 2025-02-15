/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ifexpand_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 18:23:36 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/15 16:48:07 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_initev(t_forenvv *ev, char *res, t_element *elements, t_gc *gc)
{
	(void) gc;
	ev->result = res;
	ev->elements = elements;
}

//useless
int	nobs(char c)
{
	if (c && c == '\\')
		return (1);
	return (0);
}
