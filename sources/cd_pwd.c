/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:43:09 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/23 17:05:10 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

//On a pas pris PWD directement dans les var d'env car
void	ft_pwd(t_element *elements, t_gc *gc)
{
	static char	buff[70000];

	(void) gc;
	(void) elements;
	getcwd(buff, sizeof(buff));
	ft_insert(buff, ft_strlen(buff), '\n');
	write(1, buff, ft_strlen(buff));
}

void	ft_cd(t_element *elements, t_gc *gc)
{
	
}
