/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:36:34 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/30 16:54:29 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_newlines(const char *buffer, int bytes_read)
{
	int	i;
	int	lines;

	i = 0;
	lines = 0;
	while (i < bytes_read)
	{
		if (buffer[i] == '\n')
			lines++;
		i++;
	}
	return (lines);
}

int	read_file(int fd)
{
	char	buffer[1024];
	int		bytes_read;
	int		lines;

	lines = 0;
	while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
	{
		lines += count_newlines(buffer, bytes_read);
	}
	if (bytes_read == -1)
	{
		perror("Erreur lors de la lecture du fichier");
		return (-1);
	}
	return (lines);
}


int	count_lines(const char *filename)
{
	int		fd;
	int		lines;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("Erreur lors de l'ouverture du fichier");
		return (-1);
	}
	lines = read_file(fd);
	close(fd);
	return (lines);
}

void	printf_mess_d(char *del, t_element *elements, t_gc *gc)
{
	int	nb_l;

	(void) elements;
	(void) gc;
	nb_l = count_lines(".here");
	// ft_buff_error("minishell: warning: here-document at line ", elements, gc);
	// ft_buff_error(ft_itoa(nb_l, gc), elements, gc);
	// ft_buff_error(" delimited by end-of-file (wanted `", elements, gc);
	// ft_buff_error(del, elements, gc);
	// ft_buff_error("')\n", elements, gc);
	//ft_write_error(elements, gc);
	write(2, "minishell: warning: here-document at line ", 43);
	ft_putstr_fd(ft_itoa(nb_l, gc), 2);
	write(2, " delimited by end-of-file (wanted `", 36);
	ft_putstr_fd(del, 2);
	write(2, "')\n", 4);
}

int	ft_read_heredoc(char *del, int fd, t_element *elements, t_gc *gc)
{
	char	*lign;

	while (1)
	{
		lign = readline("> ");
		if (g_signal != 0) // Si Ctrl+C est détecté, on interrompt le heredoc
		{
			elements->exit_status = ft_itoa(128 + g_signal, gc);
			//elements->nb_cmd--;
			//elements->line_valid = FALSE;
			//perror("liloo");
			g_signal = 0; // Réinitialise le signal
			free(lign);
			return (1); // Quitte la boucle
		}
		if (lign == NULL) // Si EOF (Ctrl+D) est détecté
		{
			(printf_mess_d(del, elements, gc), free(lign));
			break ;
		}
		if (ft_strcmp(lign, del) == 0) // Si le délimiteur est rencontré
		{
			free(lign);
			break;
		}
		(ft_putstr_fd(lign, fd), ft_putstr_fd("\n", fd), free(lign));
		// ft_putstr_fd("\n", fd);
		// free(lign);
	}
	return (0);
}

int	ft_open_heredoc_error(char *del, t_element *elements, t_gc *gc)
{
	int		fd;
	int		r;
	//char	*lign;

	if (access(".here", F_OK) == 0)
		unlink(".here");
	fd = open(".here", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) // changer le message d erreur
		return (perror("Error opening .here"), -1);
	ft_handle_signal(3);
	r = ft_read_heredoc(del, fd, elements, gc);
	if (r == 1)
	{
		//perror("dallas");
		elements->line_valid = FALSE;
		close(fd);
		printf("line = %d\n", elements->line_valid);
		return (-1);
	}
	//ft_heredoc_signal(gc);
	// while (1)
	// {
	// 	lign = readline("> ");
	// 	if (g_signal != 0) // Si Ctrl+C est détecté, on interrompt le heredoc
	// 	{
	// 		//perror("peach");
	// 		elements->exit_status = ft_itoa(128 + g_signal, gc);
	// 		elements->nb_cmd--;
	// 		g_signal = 0; // Réinitialise le signal
	// 		free(lign);
	// 		break ; // Quitte la boucle
	// 	}
	// 	if (lign == NULL) // Si EOF (Ctrl+D) est détecté
	// 	{
	// 		printf_mess_d(del, elements, gc);
	// 		free(lign);
	// 		break ;
	// 	}
	// 	if (ft_strcmp(lign, del) == 0) // Si le délimiteur est rencontré
	// 	{
	// 		free(lign);
	// 		break;
	// 	}
	// 	ft_putstr_fd(lign, fd);
	// 	ft_putstr_fd("\n", fd);
	// 	free(lign);
	// }
	//ft_interactive_signal(gc);
	ft_handle_signal(0);
	close(fd);
	return (fd);
}

int	ft_open_heredoc(char *del, t_element *elements, t_gc *gc)
{
	int		fd;
	int		r;
	//char	*lign;

	//perror("willis");
	if (access(".here", F_OK) == 0)
		unlink(".here");
	fd = open(".here", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) // changer le message d erreur
		return (perror("Error opening .here"), -1);
	ft_handle_signal(3);
	r = ft_read_heredoc(del, fd, elements, gc);
	if (r == 1)
	{
		//perror("dallas");
		elements->line_valid = FALSE;
		close(fd);
		//printf("line = %d\n", elements->line_valid);
		return (-1);
	}
	//ft_heredoc_signal(gc);
	// while (1)
	// {
	// 	lign = readline("> ");
	// 	if (g_signal != 0) // Si Ctrl+C est détecté, on interrompt le heredoc
	// 	{
	// 		//perror("peach");
	// 		elements->exit_status = ft_itoa(128 + g_signal, gc);
	// 		elements->nb_cmd--;
	// 		g_signal = 0; // Réinitialise le signal
	// 		free(lign);
	// 		break ; // Quitte la boucle
	// 	}
	// 	if (lign == NULL) // Si EOF (Ctrl+D) est détecté
	// 	{
	// 		printf_mess_d(del, elements, gc);
	// 		free(lign);
	// 		break ;
	// 	}
	// 	if (ft_strcmp(lign, del) == 0) // Si le délimiteur est rencontré
	// 	{
	// 		free(lign);
	// 		break;
	// 	}
	// 	ft_putstr_fd(lign, fd);
	// 	ft_putstr_fd("\n", fd);
	// 	free(lign);
	// }
	//ft_interactive_signal(gc);
	ft_handle_signal(0);
	close(fd);
	fd = open(".here", O_RDONLY);
	return (fd);
}
