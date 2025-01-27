/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open_partb.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:14:15 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/27 16:28:10 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error_out(char *name, t_element *elements, t_gc *gc)
{
	if (errno == 2)
		{
			//perror("passage par la");
			ft_buff_error("minishell: ", elements, gc);
			ft_buff_error(name, elements, gc);
			ft_buff_error(": No such file or directory\n", elements, gc);
			//write_all_err_mess(redir->name, ": No such file or directory\n", elements, gc); //il ne faut pas l afficher tout de suite
		}
		else if (errno == 13)
		{
			ft_buff_error("minishell: ", elements, gc);
			ft_buff_error(name, elements, gc);
			ft_buff_error(": Permission denied\n", elements, gc);
			//write_all_err_mess(redir->name, ": Permission denied\n", elements, gc);//il ne faut pas l afficher tout de suite
		}
		else
			perror("On a un soucis"); //ne pas oublier de l'enlever apres avoir vu tous les cas possibles
}


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

// int	count_lines(const char *filename)
// {
// 	int		fd;
// 	char	buffer[1024];
// 	int		bytes_read;
// 	int		lines;
// 	int		i;

// 	lines = 0;
// 	fd = open(filename, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("Erreur lors de l'ouverture du fichier");
// 		return (-1);
// 	}
// 	while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
// 	{
// 		i = 0;
// 		while (i < bytes_read)
// 		{
// 			if (buffer[i] == '\n')
// 				lines++;
// 			i++;
// 		}
// 	}
// 	if (bytes_read == -1)
// 	{
// 		perror("Erreur lors de la lecture du fichier");
// 		close(fd);
// 		return (-1);
// 	}
// 	close(fd);
// 	return (lines);
// }

void	printf_mess_d(char *del, t_element *elements, t_gc *gc)
{
	int	nb_l;

	nb_l = count_lines(".here");
	ft_buff_error("minishell: warning: here-document at line ", elements, gc);
	ft_buff_error(ft_itoa(nb_l, gc), elements, gc);
	ft_buff_error(" delimited by end-of-file (wanted `", elements, gc);
	ft_buff_error(del, elements, gc);
	ft_buff_error("')\n", elements, gc);
	ft_write_error(elements, gc);
}

// int	ft_open_heredoc(char *del, int flag, t_element *elements, t_gc *gc)
// {//Si le delim ne possede pas de quotes alors expand les potentiels var d'env
// 	//avec la fonction d'adri a faire aussi dans la fonction pluys bas
// 	int		fd;
// 	char	*lign;

// 	//perror("test2");
// 	if(access(".here", F_OK) == 0) // existe deja donc aura deja ete ferme avant normalement
// 		unlink(".here"); //peut on le supprimer si on a pas les droits ?
// 	fd = open(".here", O_RDWR | O_CREAT | O_TRUNC, 0644); //mettre des protection si l'ouverture ne se fait pas
// 	if (fd < 0) // verifier le message a mettre la
// 		return (perror("Error opening .here"), -1);
// 	ft_heredoc_signal(gc);
// 	// if (g_signal == 1)
// 	// 	{
// 	// 		//perror("luigi");
// 	// 		g_signal = 0;
// 	// 		return (fd);
// 	// 	}
// 	lign = readline("> ");
// 	// if (g_signal == 1)
// 	// 	{
// 	// 		//perror("luigi");
// 	// 		g_signal = 0;
// 	// 		return (free(lign), (fd));
// 	// 	}
// 	if (lign == NULL)
// 		return (printf_mess_d(del, elements, gc), free(lign), (fd));
// 	while (g_signal == 0 && lign && ft_strcmp(lign, del) != 0)
// 	{
// 		//printf("g_signal = %d\n", g_signal);
// 		ft_putstr_fd(lign, fd);
// 		ft_putstr_fd("\n", fd);
// 		free(lign);
// 		lign = readline("> ");
// 		if (g_signal == 1)
// 		{
// 			//perror("luigi");
// 			g_signal = 0;
// 			return (free(lign), (fd));
// 		}
// 		if (lign == NULL)
// 			return (printf_mess_d(del, elements, gc), free(lign), (fd));
// 	}
// 	free(lign);
// 	ft_interactive_signal(gc);
// 	close(fd);
// 	if (flag == 1)
// 		fd = open(".here", O_RDONLY); // protection
// 	//perror("test");

// 	return (fd);
// }

int	ft_open_heredoc(char *del, int flag, t_element *elements, t_gc *gc)
{
	int		fd;
	char	*lign;

	if (access(".here", F_OK) == 0)
		unlink(".here");
	fd = open(".here", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) // changer le message d erreur
		return (perror("Error opening .here"), -1);
	//ft_heredoc_signal(gc);
	while (1)
	{
		lign = readline("> ");
		if (g_signal != 0) // Si Ctrl+C est détecté, on interrompt le heredoc
		{
			//perror("peach");
			g_signal = 0; // Réinitialise le signal
			break ; // Quitte la boucle
		}
		if (lign == NULL) // Si EOF (Ctrl+D) est détecté
		{
			printf_mess_d(del, elements, gc);
			break ;
		}
		if (ft_strcmp(lign, del) == 0) // Si le délimiteur est rencontré
			break;

		ft_putstr_fd(lign, fd);
		ft_putstr_fd("\n", fd);
	}
	//ft_interactive_signal(gc);
	free(lign);
	close(fd);
	if (flag == 1)
		fd = open(".here", O_RDONLY);
	return (fd);
}

// int	ft_open_heredoc(char *del, int flag, t_element *elements, t_gc *gc)
// {
//     int fd;
//     char *lign;

//     if (access(".here", F_OK) == 0)
//         unlink(".here");
//     fd = open(".here", O_RDWR | O_CREAT | O_TRUNC, 0644);
//     ft_heredoc_signal(gc);  // Assurez-vous que cette ligne est bien ici
//     while (1)
//     {
// 		printf("g_signal dans here = %d\n", g_signal);
// 		if (g_signal == 1)
//         {
//             g_signal = 0;
//             return (free(lign), (fd));
//         }
//         lign = readline("> ");
// 		printf("g_signal dans here2= %d\n", g_signal);
//         if (g_signal == 1)
//         {
//             g_signal = 0;
//             return (free(lign), (fd));
//         }
// 		printf("g_signal dans here 3= %d\n", g_signal);
//         if (lign == NULL)
//             return (printf_mess_d(del, elements, gc), free(lign), (fd));
//         if (ft_strcmp(lign, del) == 0)
//         {
//             free(lign);
//             break;
//         }
// 		printf("g_signal dans here 4= %d\n", g_signal);
//         ft_putstr_fd(lign, fd);
//         ft_putstr_fd("\n", fd);
//         free(lign);
//     }
//     close(fd);
//     if (flag == 1)
//         fd = open(".here", O_RDONLY);
//     return (fd);
// }

// int	ft_open_heredoc_error(char *del, t_element *elements, t_gc *gc)
// {
// 	int		fd;
// 	char	*lign; //a mettre dans

// 	//perror("test2");
// 	if(access(".here", F_OK) == 0) // existe deja donc aura deja ete ferme avant normalement
// 		unlink(".here"); //peut on le supprimer si on a pas les droits ?
// 	fd = open(".here", O_RDWR | O_CREAT | O_TRUNC, 0644); //mettre des protection si l'ouverture ne se fait pas
// 	lign = readline("> ");
// 	if (lign == NULL)
// 		return (printf_mess_d(del, elements, gc), free(lign), (fd));
// 	while (lign && ft_strcmp(lign, del) != 0)
// 	{
// 		ft_putstr_fd(lign, fd);
// 		ft_putstr_fd("\n", fd);
// 		free(lign);
// 		lign = readline("> ");
// 		if (lign == NULL)
// 			return (printf_mess_d(del, elements, gc), free(lign), (fd));
// 	}
// 	free(lign);
// 	close(fd);
// 	//perror("test");
// 	return (fd);
// }


void	ft_handle_no_here_out(t_cmd *node, t_file *redir, t_element *elements, t_gc *gc)
{
	//perror("rachel");
	node->fd_in = open(redir->name, O_RDONLY, 0644);
	if (node->fd_in == ERROR_OPEN)
	{
		//perror("titeverif");
		if (errno == 2)
		{
			//perror("passage par la");
			ft_buff_error("minishell: ", elements, gc);
			ft_buff_error(redir->name, elements, gc);
			ft_buff_error(": No such file or directory\n", elements, gc);
			//write_all_err_mess(redir->name, ": No such file or directory\n", elements, gc); //il ne faut pas l afficher tout de suite
		}
		else if (errno == 13)
		{
			ft_buff_error("minishell: ", elements, gc);
			ft_buff_error(redir->name, elements, gc);
			ft_buff_error(": Permission denied\n", elements, gc);
			//write_all_err_mess(redir->name, ": Permission denied\n", elements, gc);//il ne faut pas l afficher tout de suite
		}
		else
			perror("On a un soucis"); //ne pas oublier de l'enlever apres avoir vu tous les cas possibles
		// ft_buff_error("minishell: ", elements, gc);
		// // if (elements->error)
		// // 	ft_putstr_fd(elements->error, 1);
		// ft_buff_error(redir->name, elements, gc);
		// ft_buff_error(": Permission denied\n", elements, gc); //si pas droit mais si fichier non existant le message est different
		// //perror("test");
		//printf("error juste apres buff = %s\n", elements->error);
		if (node->fd_out >= 0)
		{
			close(node->fd_out);
			node->fd_out = -3;
		}
	}
}


