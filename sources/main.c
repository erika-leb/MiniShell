/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/10 19:13:07 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

int	rl_event_handler(void)
{
	return (0);
}

volatile sig_atomic_t g_signal = 0;

char *get_input(t_gc *gc)
{
	char	*input;
	char	buffer[1024];
	ssize_t	bytes_read;
	int		len;

	if (isatty(STDIN_FILENO))
	{
		fflush(stdout);
		input = readline("minishell> ");
		//fflush(stdout);
		if (input && *input)
			add_history(input);
	}
	else
	{
		bytes_read = read(STDIN_FILENO, buffer, 1023);
		if (bytes_read <= 0)
			return NULL;
		buffer[bytes_read] = '\0';
		len = 0;
		while (buffer[len] && buffer[len] != '\n')
			len++;
		buffer[len] = '\0';
		input = ft_strdup(buffer, gc);
	}
	return input;
}

void	ft_init(t_element *elements, char **env, t_gc *gc, char **av)
{
	ft_welcome();
	ft_cpy_env(elements, env, gc, av);
	reset_signal_status();
	ft_handle_signal(0);
	rl_event_hook = rl_event_handler;
}

void	ft_launch_cmd(t_element *elements, t_gc *gc)
{
	ft_fill_arrays(elements, gc);
	//perror("bolosskikou");
	pipe_creation(elements, gc);
	//perror("boloss");
	//check_fds("parent au debut");
	child_creation(elements, gc);
	//perror("bolosskikou");
	close_pipes(elements);
	//perror("kikoulol");
	wait_for_children(elements, gc);
	//perror("wtf");
	if(access(".here", F_OK) == 0) // existe deja donc aura deja ete ferme avant normalement
		unlink(".here"); //peut on le supprimer si on a pas les droits ?
	reset_signal_status();
	ft_handle_signal(0);
}


 int	main(int ac, char **av, char **env)
 {
	t_element	*elements;
	t_gc		gc;

	(void)ac;
	ft_handle_signal(2);
	gc_init(&gc);
	elements = ft_init_struct(&gc);
	ft_init(elements, env, &gc, av);
	while (1)
	{
		elements->lst = NULL;
		elements->line = get_input(&gc);
		//perror("harry");
		if (elements->line == NULL) // ctrl d
			(gc_cleanup(&gc), exit(EXIT_SUCCESS));
		// perror("thierry");
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
				ft_launch_cmd(elements, &gc);
		}
		elements->line_valid = TRUE;
	}
	return (0);
}
