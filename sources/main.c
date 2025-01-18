/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/18 17:08:12 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sigint(int sig)
{
	(void) sig;
	write(1, "\nminishell> ", 12);
}

void	ft_welcome(void)
{
	printf("\n");
	printf(COLOR_CYAN "******************************\n" COLOR_RESET);
	printf(COLOR_MAGENTA "*  " COLOR_YELLOW "Bienvenue dans Minishell" COLOR_MAGENTA "  *\n" COLOR_RESET);
	printf(COLOR_CYAN "******************************\n" COLOR_RESET);
	printf("\n");
}

void	ft_signal_handle(char *line)
{
	struct sigaction	sa;
	struct sigaction	sa_bis;

	sa.sa_handler = &handle_sigint;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		free(line);
		perror("sigaction");
		exit(1);
	}
	sa_bis.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa_bis, NULL) == -1)
	{
		free(line);
		perror("sigaction");
		exit(1);
	}
}

int	main(void)
{
	char	*line;
	char	**result;
	int		i = 0;
	
	line = NULL;
	result = NULL;
	ft_signal_handle(line);
	ft_welcome();
	while (1)
	{
		line = readline("minishell> ");
		if (line == NULL)
		{
			free(line);
			printf("exit\n");
			return (0); // rajouter le clean
		}
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			printf("exit\n");
			return (0); // rajouter le clean
		}
		if (line && *line)
			add_history(line);
		ft_ft(line, result);
		free(line);
		ft_freesplit(result, i);//??
	}
	return (0);
}