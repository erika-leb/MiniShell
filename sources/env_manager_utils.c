/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:00:44 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/13 15:02:21 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static int	ft_merrorexp(char *name_key, t_element *elements, t_gc *gc)
{
	ft_buff_error("minishell: export: `", elements, gc);
	ft_buff_error(name_key, elements, gc);
	ft_buff_error("': not a valid identifier\n", elements, gc);
	ft_write_error(elements, gc);
	return (1);
}

int   ft_exparser(char *name_key, t_element *elements, t_gc *gc)
{
	int i;

	if (!ft_isalpha(name_key[0]) && name_key[0] != '_')
		return (ft_merrorexp(name_key, elements, gc)); //changer le message et le exit status
	i = 0;
	while (name_key[++i])
	{
		if (name_key[i] == '+' && name_key[i + 1] == '=')
			break;
		if (name_key[i] == '=')
			break;
		if (!ft_isalnum(name_key[i]) && name_key[i] != '_')
			return(ft_merrorexp(name_key, elements, gc));
	}
	return (0);
}