/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 17:51:40 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/16 21:24:30 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_sigint(int sig)
{
	(void) sig;
	rl_done = 1;
	g_signal = sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	handle_sigint_here(int sig)
{
	(void) sig;
	rl_done = 1;
	g_signal = sig;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	handle_sigint_in_process(int sig)
{
	(void) sig;
	write(2, "\n", 1);
	rl_done = 1;
	g_signal = sig;
}

static void	handle_sigquit_in_process(int sig)
{
	(void)sig;
	g_signal = sig;
}

void	ft_handle_signal(int process)
{
	if (process == 1)
	{
		signal(SIGINT, handle_sigint_in_process);
		signal(SIGQUIT, handle_sigquit_in_process);
		signal(SIGPIPE, handle_sigpipe);
	}
	else if (process == 0)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGPIPE, handle_sigpipe);
	}
	else if (process == 2)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (process == 3)
	{
		signal(SIGINT, handle_sigint_here);
		signal(SIGQUIT, SIG_IGN);
	}
	signal(SIGTSTP, SIG_IGN);
	signal(SIGPIPE, handle_sigpipe); // a garder ?
	//signal(SIGPIPE, SIG_IGN); // a garder ?
}

