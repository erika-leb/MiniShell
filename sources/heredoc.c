/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:36:34 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/11 14:37:32 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// static char	*here_exp(char *lign, t_element *elements, t_gc *gc)
// {
// 	char	*buffer;

// 	buffer = gc_malloc(ft_strlen(lign) + 1, gc); //voir correction leak avec adri ici, si expand la taille n est pas bonne
// 	ft_strncpy(buffer, lign, ft_strlen(lign));
// 	buffer[ft_strlen(lign)] = '\0';
// 	//printf("buff = %s\n", buffer);
// 	return (ft_hereifexpand(buffer, elements, gc));

// }

// static int	ft_isquote(char *del)
// {
// 	int	i;

// 	i = 0;
// 	while (del[i])
// 	{
// 		if (del[i] == '\'' || del[i] == '\"')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

static char	*here_exp(char *lign, t_element *elements, t_gc *gc, int flag)
{
	static char	buffer[70000];
	size_t len;

	if (!lign)
		return (NULL);
	if (flag)
		return (lign);
	// S'assurer que lign ne dépasse pas la taille de buffer
	len = ft_strlen(lign);
	if (len >= sizeof(buffer))
		len = sizeof(buffer) - 1; // Éviter un dépassement
	// Copier lign dans buffer en toute sécurité
	ft_strncpy(buffer, lign, len);
	buffer[len] = '\0';

	// Appeler ft_hereifexpand avec buffer
	return (ft_hereifexpand(buffer, elements, gc));
}


int	ft_read_heredoc(char *del, int fd, t_element *elements, t_gc *gc)
{
	char	*lign;
	char	*dell;
	int		flag;

	flag = 0;
	dell = ft_concat(ft_strdup(del, gc), -1, 0, 0);
	if (ft_strcmp(dell, del))
		flag = 1;
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
			(printf_mess_d(dell, elements, gc), free(lign));
			break ;
		}
		if (ft_strcmp(lign, dell) == 0) // Si le délimiteur est rencontré
		{
			free(lign);
			break;
		}
		// (ft_putstr_fd(lign, fd), ft_putstr_fd("\n", fd), free(lign));
		(ft_putstr_fd(here_exp(lign, elements, gc, flag), fd), ft_putstr_fd("\n", fd), free(lign));
		// ft_putstr_fd(here_exp(lign, elements, gc), fd);
		// ft_putstr_fd("\n", fd);
		// free(lign);
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
	{
		ft_buff_error("minishell: ", elements, gc);
		ft_buff_error(name, elements, gc);
		ft_buff_error(": ", elements, gc);
		ft_buff_error(strerror(errno), elements, gc);
		ft_buff_error("\n", elements, gc);
		//perror("on a un soucis");
		//ft_write_error(elements, gc);
	}
	elements->exit_status = ft_itoa(1, gc);
}
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
		{
			ft_buff_error("minishell: ", elements, gc);
			ft_buff_error(redir->name, elements, gc);
			ft_buff_error(": ", elements, gc);
			ft_buff_error(strerror(errno), elements, gc);
			ft_buff_error("\n", elements, gc);
			//perror("on a un soucis");
			//ft_write_error(elements, gc);
		}
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
		elements->exit_status = ft_itoa(1, gc);
	}
}
