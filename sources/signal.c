/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:03:56 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/17 18:48:47 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_sigint(int sig)
{
	(void) sig;
	write(1, "\nminishell> ", 12);
}

void	ft_signal_handle(char *line)
{
	struct sigaction	sa;
	struct sigaction	sa_bis;

	sa.sa_handler = &handle_sigint;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		free(line);
		clear_history();
		perror("sigaction");
		exit(1);
	}
	sa_bis.sa_handler = SIG_IGN;
	sigemptyset(&sa_bis.sa_mask);
	if (sigaction(SIGQUIT, &sa_bis, NULL) == -1)
	{
		free(line);
		clear_history();
		perror("sigaction");
		exit(1);
	}
}
