/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 17:51:40 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/18 19:13:07 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);          // Ajoute un saut de ligne
	rl_on_new_line();           // Prépare une nouvelle ligne
	rl_replace_line("", 0);     // Efface la ligne en cours
	rl_redisplay();             // Redessine le prompt
}

void	ft_signal_handle(t_gc *gc)
{
	struct sigaction	sa;
	struct sigaction	sa_bis;

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = &handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	//	ft_error_exit("Error: sigaction failed", elements, -1, PERROR);
	memset(&sa_bis, 0, sizeof(struct sigaction));
	sa_bis.sa_handler = SIG_IGN;
	sigemptyset(&sa_bis.sa_mask);
	sa_bis.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_bis, NULL) == -1)
	{
		perror("sigaction failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
		//ft_error_exit("Error: sigaction failed", elements, -1, );
}
