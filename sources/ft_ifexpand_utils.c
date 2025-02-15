/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ifexpand_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 18:23:36 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/15 16:15:48 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_initev(t_forenvv *ev, char *res, t_element *elements, t_gc *gc)
{
	(void) gc;
	ev->result = res;
	ev->elements = elements;
}
