/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/20 19:03:58 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

 int	main(void)
 {
 	char		*line;
 	char		**result;
	t_element	*elements;
 	int			i = 0;

 	line = NULL;
	elements = ft_init_struct();
 	ft_signal_handle(elements);
 	ft_welcome();
 	while (1)
 	{
 		line = readline("minishell> ");
 		if (line == NULL)
 			ft_error_exit("", elements, 0, NO_PERROR);
 		if (ft_strcmp(line, "exit") == 0)
 		{
			ft_error_exit("exit\n", elements, 0, NO_PERROR);
 		}
 		if (line && *line)
 			add_history(line);
 		result = ft_split(ft_tokenize(line), ' ', 0, 0);
 		while (result[i])
  		{
  			printf("token %d : %s\n", i, result[i]);
 			i++;
  		}
 		free(line);
 	}
	ft_error_exit("", elements, 0, NO_PERROR);
 	return (0);
 }

// int	main(int ac, char **av)
// {
// 	char	**result;
// 	int		i;
// 	(void) ac;

// 	result = ft_split(ft_tokenize(av[1]), av[2][0], 0, 0);
// 	if (result == NULL)
// 		return (printf("Erreur : ft_split a renvoyé NULL\n"));
// 	i = 0;
// 	while (result[i])
// 	{
// 		printf("token %d : %s\n", i, result[i]);
// 		i++;
// 	}
// 	ft_freesplit(result, i);
// 	return (0);
// }
