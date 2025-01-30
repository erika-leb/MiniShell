/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/30 17:08:59 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

int	rl_event_handler(void)
{
	return (0);
}

volatile sig_atomic_t g_signal = 0;

 int	main(int ac, char **av, char **env)
 {
	t_element	*elements;
	t_gc		gc;

	((void)ac, (void)av);
	//ft_ignore_signal(&gc);
	ft_handle_signal(2);
	gc_init(&gc);

	//ft_ignore_signal(&gc); //si je fais ctrl c tres rapidement apres avoirlancer le programme j ai des soucis
	//dprintf(2, "kikouicham\n");
	//perror("avant init");
	elements = ft_init_struct(&gc);
 	//ft_signal_handle(&gc);
	//ft_welcome();
	ft_cpy_env(elements, env, &gc);
	reset_signal_status();
	ft_handle_signal(0);
	//ft_interactive_signal(&gc);
	rl_event_hook = rl_event_handler;
	while (1)
	{
		//perror("henry");
		//ft_interactive_signal(&gc);
		elements->lst = NULL;
		//perror("styles");
		// if (isatty(fileno(stdin)))
		// 	elements->line = readline(shell->terminal_prompt);
		//elements->line = readline("minishell> ");
		elements->line = readline("minishell> ");
		//perror("harry");
		if (elements->line == NULL) // c est pour ctrl d je crois
			(gc_cleanup(&gc), exit(EXIT_SUCCESS));
		if (elements->line && *(elements->line))
			add_history(elements->line);
		// printf("line = %s vgbgbh\n", elements->line);
		// perror("thierry");
		if  (ft_strcmp(elements->line, "") != 0) //ligne de merde pour eviter le segfault dans hegde case 1 si je fais entree; demander correction a adrien
		{
			ft_ft(elements, &gc);
	// printf("earr[0] = %sdfvf\n", elements->arr[0]);
			// perror("jerry");
			if (elements->arr)
			{
				// perror("test2");
				lexing(elements->arr, &elements->lst, elements, &gc);
				// perror("kikoulol");
				//printf("line amin= %d\n", elements->line_valid);
				if (elements->line_valid == TRUE)
				{
					ft_fill_arrays(elements, &gc);
					//perror("bolosskikou");
					pipe_creation(elements, &gc);
					//perror("boloss");
					//check_fds("parent au debut");
					child_creation(elements, &gc);
					//perror("bolosskikou");
					close_pipes(elements);
					//perror("kikoulol");
					wait_for_children(elements, &gc);
					//perror("wtf");
					//printf("errno = %d\n", errno);
					if(access(".here", F_OK) == 0) // existe deja donc aura deja ete ferme avant normalement
						unlink(".here"); //peut on le supprimer si on a pas les droits ?
					//printf("exit_status = %s\n", elements->exit_status);
					//elements->exit_status = ft_itoa(errno, &gc);
					//check_fds("parent a la fin");
					reset_signal_status();
					ft_handle_signal(0);
				}
			}
		}
		elements->line_valid = TRUE;
	}
	// gc_cleanup(&gc); // utile ?
	// exit(EXIT_SUCCESS); // utile ?
	return (0);
}

