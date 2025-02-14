/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:55:44 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/14 15:54:48 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_istok_(char *av2)
{
	if (!ft_strcmp(av2, "|") || !ft_strcmp(av2, "<") || !ft_strcmp(av2, ">")
		|| !ft_strcmp(av2, "<<") || !ft_strcmp(av2, ">>"))
		return (1);
	return (0);
}

void	ft_deldollar(char *input)
{
	int	sq;
	int	dq;
	int	i;

	sq = 0;
	dq = 0;
	i = 0;
	while (input[i])
	{
		ft_modifquote_(input, &sq, &dq, &i);
		if (!sq && !dq && input[i] == '$'
			&& (input[i + 1] == '\'' || input[i + 1] == '\"'))
			ft_erase(input, i);
		i++;
	}
}
