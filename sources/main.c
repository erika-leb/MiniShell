/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/22 15:20:22 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

 int	main(int ac, char **av, char **env)
 {
	t_element	*elements;
	t_gc		gc;

	((void)ac, (void)av);
	gc_init(&gc);
	elements = ft_init_struct(&gc);
 	ft_signal_handle(&gc);
 	ft_welcome();
	ft_cpy_env(elements, env, &gc);
 	while (1)
 	{
		elements->lst = NULL;
 		elements->line = readline("minishell> ");
 		if (elements->line == NULL)
			(gc_cleanup(&gc), exit(EXIT_SUCCESS));
 		if (elements->line && *(elements->line))
 			add_history(elements->line);
		ft_ft(elements, &gc);
		if (elements->arr)
		{
			//perror("test2");
			//lexing(elements, &gc);
			lexing(elements->arr, &elements->lst, elements, &gc);
			//perror("kikoulol");
			ft_fill_arrays(elements, &gc);
			//perror("bolosskikou");
			pipe_creation(elements, &gc);
			//perror("boloss");
			//check_fds("parent au debut");
			child_creation(elements, &gc);
			//perror("bolosskikou");
			close_pipes(elements);
			//perror("kikoulol");
			wait_for_children(elements);
			//perror("wtf");
			//perror("test3");
			printf("errno = %d\n", errno);
			if(access(".here", F_OK) == 0) // existe deja donc aura deja ete ferme avant normalement
				unlink(".here"); //peut on le supprimer si on a pas les droits ?
			//check_fds("parent a la fin");
		}
 	}
	gc_cleanup(&gc); // utile ?
	exit(EXIT_SUCCESS); // utile ?
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
