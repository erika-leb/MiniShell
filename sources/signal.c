/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 17:51:40 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/06 15:21:47 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

// static void	handle_sigint(int sig) // ancien
// {
// 	(void)sig;
// 	//perror("mario");
// 	//write(1, "\n", 1);          // Ajoute un saut de ligne
// 	rl_on_new_line();           // Prépare une nouvelle ligne
// 	rl_replace_line("", 0);     // Efface la ligne en cours
// 	rl_redisplay();             // Redessine le prompt
// }

static void	handle_sigint(int sig)
{
	(void) sig;

	rl_done = 1;
	rl_on_new_line();           // Prépare une nouvelle ligne
	rl_replace_line("", 0);     // Efface la ligne en cours
	rl_redisplay();             // Redessine le prompt
	//g_signal = sig;
	// rl_on_new_line();           // Prépare une nouvelle ligne
	// rl_replace_line("", 0);     // Efface la ligne en cours
	// rl_redisplay();             // Redessine le prompt
	//printf("g_signa=%d\n", g_signal);
}

static void	handle_sigint_here(int sig)
{
	(void) sig;

	rl_done = 1;
	g_signal = sig;
	rl_on_new_line();           // Prépare une nouvelle ligne
	rl_replace_line("", 0);     // Efface la ligne en cours
	rl_redisplay();             // Redessine le prompt
	//printf("g_signa=%d\n", g_signal);
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
	//write(2, "Quit (core dumped)\n", 20);
	g_signal = sig;
}

void	reset_signal_status(void)
{
	rl_done = 0;
	g_signal = 0;
}

void handle_sigpipe(int sig)
{
	g_signal = sig;
}

void	ft_handle_signal(int process)
{
	//dprintf(2, "Last mode is [%d]\n", process);
	if (process == 1)
	{
		signal(SIGINT, handle_sigint_in_process); // ctrl + c
		signal(SIGQUIT, handle_sigquit_in_process); // ctrl +
		signal(SIGTSTP, SIG_IGN);
		signal(SIGPIPE, handle_sigpipe);
	}
	else if (process == 0)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
		signal(SIGPIPE, handle_sigpipe);
	}
	else if (process == 2) // ignore
	{
		//perror("gloria");
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
	else if (process == 3) // heredoc
	{
		signal(SIGINT, handle_sigint_here);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
}

// static void	handle_sigint_bis(int sig)
// {
// 	(void)sig;
// 	//perror("bros");
// 	g_signal = 1;
// 	rl_done = 1;  // Indique que readline doit terminer la lecture

//     // On peut également forcer un retour à la ligne et redessiner le prompt
//     //write(STDIN_FILENO, "\n", 1);  // Pour simuler un retour à la ligne
//    // rl_replace_line("", 0);         // Efface la ligne
//     //rl_on_new_line();               // Prépare une nouvelle ligne
//     //rl_redisplay();
// 	// write(1, "\n", 1);          // pas sur que ce soit le bon fd
// 	// //rl_replace_line("", 0);
//     // rl_replace_line("", 0);
//     // rl_on_new_line();  // Préparer readline pour la prochaine ligne
//     // rl_redisplay();

// 	// rl_replace_line("", 0);  // Efface la ligne actuelle dans readline
// 	// rl_on_new_line();        // Prépare une nouvelle ligne sans saut de ligne
// 	// rl_redisplay();
// 	write(STDOUT_FILENO, "\n", 1);
// 	//ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 	// rl_on_new_line();           // Prépare une nouvelle ligne
// 	// rl_replace_line("", 0);     // Efface la ligne en cours
// 	// rl_redisplay();             // Redessine le prompt
// 	//printf("g_signal = %d\n", g_signal);
// }

// static void	handle_sigint_tree(int sig)
// {
// 	(void)sig;
// 	//perror("wario");
// 	//g_signal = 1;
//     //rl_done = 1;  // Indique que readline doit terminer la lecture

//     // On peut également forcer un retour à la ligne et redessiner le prompt
//     //write(STDIN_FILENO, "\n", 1);  // Pour simuler un retour à la ligne
//    // rl_replace_line("", 0);         // Efface la ligne
//     //rl_on_new_line();               // Prépare une nouvelle ligne
//     //rl_redisplay();
// 	// write(1, "\n", 1);          // pas sur que ce soit le bon fd
// 	// //rl_replace_line("", 0);
//     // rl_replace_line("", 0);
//     // rl_on_new_line();  // Préparer readline pour la prochaine ligne
//     // rl_redisplay();
// 	write(1, "\n", 1);
// 	rl_replace_line("", 0);  // Efface la ligne actuelle dans readline
// 	rl_on_new_line();        // Prépare une nouvelle ligne sans saut de ligne
// 	rl_redisplay();

// 	ioctl(STDIN_FILENO, TIOCSTI, "\n");
// 	// rl_on_new_line();           // Prépare une nouvelle ligne
// 	// rl_replace_line("", 0);     // Efface la ligne en cours
// 	// rl_redisplay();             // Redessine le prompt
// 	//printf("g_signal = %d\n", g_signal);
// }

// static void	handle_sigquit(int sig)
// {
// 	(void)sig;
// 	// perror("lilo");
// 	// printf("anya");
// 	//printf("^\Quit (core dumped)\\n");
// 	//write(1, "Quit (core dumped)\n", 20);
// 	rl_on_new_line();           // Prépare une nouvelle ligne
//     rl_replace_line("", 0);     // Efface la ligne en cours
//     rl_redisplay();             // Redessine le prompt
// 	//rl_done = 1;
// 	//ioctl(STDIN_FILENO, TIOCSTI, "");
// }

// void	ft_interactive_signal(t_gc *gc) //interactif
// {
// 	//perror("carlito");
// 	if (signal(SIGINT, handle_sigint) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// 	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// 	if (signal(SIGTSTP, SIG_IGN) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// }

// void	ft_heredoc_signal(t_gc *gc) //heredoc il faut mettre le signal pour savoir qu il faut sortir du heredoc
// {
// 	//perror("doc");
// 	if (signal(SIGINT, handle_sigint_bis) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// 	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// 	if (signal(SIGTSTP, SIG_IGN) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// }

// void	ft_ignore_signal(t_gc *gc)
// {
// 	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// 	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// 	if (signal(SIGTSTP, SIG_IGN) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// }

// void	ft_exec_signal(t_gc *gc) //exec
// {
// 	//perror("mcfly");
// 	if (signal(SIGINT, handle_sigint_tree) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// 	if (signal(SIGQUIT, handle_sigquit) == SIG_ERR)
// 		(perror("signal failed"), gc_cleanup(gc), exit(EXIT_FAILURE));
// }
































// void	ft_exec_signal(t_gc *gc) //exec
// {
// 	struct sigaction	sa;
// 	struct sigaction	sa_bis;

// 	memset(&sa, 0, sizeof(struct sigaction));
// 	sa.sa_handler = &handle_sigint_tree;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	if (sigaction(SIGINT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction failed"); //autre message a mettre
// 		gc_cleanup(gc);
// 		exit(EXIT_FAILURE);
// 	}
// 	//	ft_error_exit("Error: sigaction failed", elements, -1, PERROR);
// 	memset(&sa_bis, 0, sizeof(struct sigaction));
// 	sa_bis.sa_handler = &handle_sigquit;
// 	sigemptyset(&sa_bis.sa_mask);
// 	sa_bis.sa_flags = 0;
// 	if (sigaction(SIGQUIT, &sa_bis, NULL) == -1)
// 	{
// 		perror("sigaction failed");
// 		gc_cleanup(gc);
// 		exit(EXIT_FAILURE);
// 	}
// 		//ft_error_exit("Error: sigaction failed", elements, -1, );
// }

// void	ft_heredoc_signal(t_gc *gc) //heredoc
// {
// 	struct sigaction	sa;
// 	struct sigaction	sa_bis;

// 	memset(&sa, 0, sizeof(struct sigaction));
// 	sa.sa_handler = &handle_sigint_bis;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	if (sigaction(SIGINT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction failed"); //autre message a mettre
// 		gc_cleanup(gc);
// 		exit(EXIT_FAILURE);
// 	}
// 	//	ft_error_exit("Error: sigaction failed", elements, -1, PERROR);
// 	memset(&sa_bis, 0, sizeof(struct sigaction));
// 	sa_bis.sa_handler = SIG_IGN;
// 	sigemptyset(&sa_bis.sa_mask);
// 	sa_bis.sa_flags = 0;
// 	if (sigaction(SIGQUIT, &sa_bis, NULL) == -1)
// 	{
// 		perror("sigaction failed");
// 		gc_cleanup(gc);
// 		exit(EXIT_FAILURE);
// 	}
// 		//ft_error_exit("Error: sigaction failed", elements, -1, );
// }

// void	ft_interactive_signal(t_gc *gc) //interatif
// {
// 	struct sigaction	sa;
// 	struct sigaction	sa_bis;

// 	memset(&sa, 0, sizeof(struct sigaction));
// 	sa.sa_handler = &handle_sigint;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	if (sigaction(SIGINT, &sa, NULL) == -1)
// 	{
// 		perror("sigaction failed"); //autre message a mettre
// 		gc_cleanup(gc);
// 		exit(EXIT_FAILURE);
// 	}
// 	//	ft_error_exit("Error: sigaction failed", elements, -1, PERROR);
// 	memset(&sa_bis, 0, sizeof(struct sigaction));
// 	sa_bis.sa_handler = SIG_IGN;
// 	sigemptyset(&sa_bis.sa_mask);
// 	sa_bis.sa_flags = 0;
// 	if (sigaction(SIGQUIT, &sa_bis, NULL) == -1)
// 	{
// 		perror("sigaction failed");
// 		gc_cleanup(gc);
// 		exit(EXIT_FAILURE);
// 	}
// 		//ft_error_exit("Error: sigaction failed", elements, -1, );
// }
