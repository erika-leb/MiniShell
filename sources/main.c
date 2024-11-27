/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/27 17:56:30 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

void	ft_cpy_env(t_element *elements, char **env, t_gc *gc)
{
	int	s_arr;
	int	i;
	int	s;

	s_arr = ft_arr_size(env);
	elements->env = gc_malloc(sizeof(char *) * (s_arr + 1), gc);
	i = 0;
	while (env[i])
	{
		s = ft_strlen(env[i]);
		elements->env[i] = gc_malloc(s + 1, gc);
		elements->env[i] = env[i];
		i++;
	}
	elements->env[i] = NULL;

	// i = 0;
	// while( i <= s_arr)
	// {
	// 	printf("env %i = %s\n", i, env[i]);
	// 	printf("myenv %i = %s\n", i, elements->env[i]);
	// 	i++;
	// }
}

 int	main(int ac, char **av, char **env)
 {
 //	char		**result;
	t_element	*elements;
	t_gc		gc;
	//char  *line;
	
	((void)ac, (void)av);
	gc_init(&gc);
	elements = ft_init_struct(&gc);
 	ft_signal_handle(&gc);
 	ft_welcome();
	ft_cpy_env(elements, env, &gc);
	//elements->env = env; // a changer plus tard en dupliquant
 	while (1)
 	{
 		elements->line = readline("minishell> ");
 		if (elements->line == NULL)
			(gc_cleanup(&gc), exit(EXIT_SUCCESS));
 			//ft_error_exit("", 0, NO_PERROR);
 		if (ft_strcmp(elements->line, "exit") == 0)
		//	ft_error_exit("exit\n", 0, NO_PERROR);
		{
			write(1, "exit\n", 6);
			(gc_cleanup(&gc), exit(EXIT_SUCCESS));
		}
 		if (elements->line && *(elements->line))
 			add_history(elements->line);
		elements->arr = ft_split(ft_tokenize(elements->line), 0, 0, &gc);
		//result = ft_split(ft_concat(ft_tokenize(line), 0, 0), ' ', 0, 0);
		
		int i = 0;
		// int s = ft_arr_size(elements->arr);
		// printf("i = %d, s = %d\n", i, s);
		//while (i < s - 1)
		while(elements->arr[i])
 		{
			elements->arr[i] = ft_concat(elements->arr[i], -1, 0, 0);
 			//printf("i = %d, s = %d\n", i, s);
			printf("token %d :%s\n", i, elements->arr[i]);
			i++;
 		}
		//perror("test2");
		//lexing(elements, &gc);
		lexing(elements->arr, &elements->lst, &gc);
		//perror("test3");
 	}
	//ft_error_exit("", 0, NO_PERROR);
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
