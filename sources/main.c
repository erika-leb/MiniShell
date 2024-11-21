/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/21 19:58:36 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

//  int	main(void)
//  {
//  	char		*line;
//  //	char		**result;
// 	//t_element	*elements;
//  	//int			i = 0;
// 	t_gc		gc;

//  	line = NULL;
// 	gc_init(&gc);
// 	//elements = ft_init_struct();
//  	ft_signal_handle(&gc);
//  	ft_welcome();
//  	while (1)
//  	{
//  		line = readline("minishell> ");
//  		if (line == NULL)
// 		{
// 			gc_cleanup(&gc);
// 			exit(EXIT_SUCCESS);
// 		}
//  			//ft_error_exit("", 0, NO_PERROR);
//  		if (ft_strcmp(line, "exit") == 0)
// 		//	ft_error_exit("exit\n", 0, NO_PERROR);
// 		{
// 			write(1, "exit\n", 6);
// 			gc_cleanup(&gc);
// 			exit(EXIT_SUCCESS);
// 		}
//  		if (line && *line)
//  			add_history(line);
//  	//	result = ft_split(ft_tokenize(line), ' ', 0, 0);
		
//  		// while (result[i])
//   		// {
//   		// 	printf("token %d : %s\n", i, result[i]);
//  		// 	i++;
//   		// }
//  		free(line);
//  	}
// 	//ft_error_exit("", 0, NO_PERROR);
// 	gc_cleanup(&gc); // utile ?
// 	exit(EXIT_SUCCESS); // utile ?
//  	return (0);
//  }

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
