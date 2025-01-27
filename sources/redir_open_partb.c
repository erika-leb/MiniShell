/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open_partb.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:14:15 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/27 17:42:08 by ele-borg         ###   ########.fr       */
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


