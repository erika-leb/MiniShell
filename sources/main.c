/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/16 17:06:30 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int main(int ac, char **av)
{
	char	**result;
	int		i;
	(void) ac;
	
	result = ft_split(av[1], av[2][0], 0, 0);
	if (result == NULL)
		return(printf("Erreur : ft_split a renvoyé NULL\n"));
	printf("Résultat de ft_split :\n");
	i = 0;
	while (result[i])
	{
		printf("token %d : %s\n", i, result[i]);
		i++;
	}
	ft_freesplit(result, i);
	return (0);
}
