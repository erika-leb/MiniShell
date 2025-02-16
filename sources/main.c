/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/16 16:11:54 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	rl_event_handler(void)
{
	return (0);
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
	lexing(elements->arr, &elements->lst, elements, gc);
	// perror("kikoulol");
	if (elements->line_valid == TRUE)
	{
		ft_fill_arrays(elements, gc);
		// perror("bolosskikou");
		pipe_creation(elements, gc);
		// perror("boloss");
		// check_fds("parent au debut");
		child_creation(elements, gc);
		// perror("bolosskikou");
		close_pipes(elements);
		// perror("kikoulol");
		wait_for_children(elements, gc);
		// perror("wtf");
		if (access(".here", F_OK) == 0)
			unlink(".here");
		reset_signal_status();
		//(0);
	}
}

void	end_of_handle(t_element *elements, t_gc *gc)
{
	if (g_signal != 0)
	{
		elements->exit_status = ft_itoa(128 + g_signal, gc);
		g_signal = 0;
	}
	elements->line_valid = TRUE;
}

int	main(int ac, char **av, char **env)
{
	t_element	*elements;
	t_gc		gc;

	(void)ac;
	// ft_handle_signal(2);
	gc_init(&gc);
	elements = ft_init_struct(&gc);
	ft_init(elements, env, &gc, av);
	while (1)
	{
		elements->lst = NULL;
		elements->line = get_input(&gc);
		// perror("harry");
		if (elements->line == NULL)
		{
			if (env[0] == NULL)
				write(1, "\n", 1);
			(gc_cleanup(&gc), exit(EXIT_SUCCESS));
		}
		// perror("thierry");
		ft_ft(elements, &gc);
		// perror("jerry");
		if (elements->arr)
			ft_launch_cmd(elements, &gc);
		// {
		// 	//perror("test2");
		// 	lexing(elements->arr, &elements->lst, elements, &gc);
		// 		// perror("kikoulol");
		// 	if (elements->line_valid == TRUE)
		// 		ft_launch_cmd(elements, &gc);
		// }
		end_of_handle(elements, &gc);
		// if (g_signal != 0)
		// {
		// 	elements->exit_status = ft_itoa(128 + g_signal, &gc);
		// 	g_signal = 0;
		// }
		// elements->line_valid = TRUE;
	}
	return (0);
}
