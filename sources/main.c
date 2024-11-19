/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/18 15:17:36 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_welcome(void)
{
	printf("\n");
	printf(CCYAN "******************************\n" CRESET);
	printf(CMAGENTA "*  " CYELLOW "Minishell" CMAGENTA "  *\n" CRESET);
	printf(CCYAN "******************************\n" CRESET);
	printf("\n");
}

// void handle_line(int ac, char **av)
// {
// 	char	**result;
// 	int		i;
// 	(void) ac;
	
// 	result = ft_split(av[1], av[2][0], 0, 0);
// 	if (result == NULL)
// 		return(printf("Erreur : ft_split a renvoyé NULL\n"));
// 	printf("Résultat de ft_split :\n");
// 	i = 0;
// 	while (result[i])
// 	{
// 		printf("token %d : %s\n", i, result[i]);
// 		i++;
// 	}
// 	ft_freesplit(result, i);
// 	return (0);
// }

// int	main(void)
// {
// 	char	*line;
	
// 	line = NULL;
// 	ft_signal_handle(line);
// 	ft_welcome();
// 	while (1)
// 	{
// 		line = readline("minishell> ");
// 		if (line == NULL || ft_strcmp(line, "exit") == 0)
// 		{
// 			free(line);
// 			clear_history();
// 			printf("exit\n");
// 			return (0); // rajouter le clean
// 		}
// 		if (line && *line)
// 			add_history(line);
// 		//handle_line(1, line);
// 		free(line);
// 		clear_history();
// 	}
// 	return (0);
// }
