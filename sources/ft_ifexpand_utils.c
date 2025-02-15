/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ifexpand_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 18:23:36 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/14 18:47:17 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_initev(t_forenvv *ev, char *res, t_element *elements, t_gc *gc)
{
	ev->result = ft_strdup_(res, gc);
	ev->elements = elements;
}
