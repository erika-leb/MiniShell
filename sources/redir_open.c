/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:14:15 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/28 18:22:17 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_open_heredoc(char *del, int here)
{
	int		fd;
	char	*lign; //a mettre dans
	(void)	here;

	if(access(".here", F_OK) == 0) // existe deja donc aura deja ete ferme avant normalement
		unlink(".here"); //peut on le supprimer si on a pas les droits ?
	fd = open(".here", O_WRONLY | O_CREAT , 0644);
	lign = readline("> ");
	if (lign == NULL)
		return (free(lign), (fd));
	while (ft_strcmp(lign, del) != 0)
	{
		ft_putstr_fd(lign, fd);
		ft_putstr_fd("\n", fd);
		free(lign);
		lign = readline("> ");
		if (lign == NULL)
		{
			free(lign);
			return (fd);
		}
	}
	//ft_putstr_fd(lign, fd);
	free(lign);
	return (fd);
}

void	ft_fd_open(t_cmd *node)
{
	t_file	*redir;

	redir = node->redir;
	while(redir)
	{
		if (redir->token == TRUNC || redir->token == APPEND)
		{
			if (node->fd_out != ERROR_OPEN && node->fd_in != ERROR_OPEN)
			{
				if (node->fd_out >= 0)
					close(node->fd_out);
				if (redir->token == TRUNC)
					node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				else
					node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (node->fd_out == ERROR_OPEN)
				{
					perror("Error");
					if (node->fd_in >= 0)
					{
						close(node->fd_in);
						if(access(".here", F_OK) == 0) // existe deja donc aura deja ete ferme avant normalement
							unlink(".here"); //peut on le supprimer si on a pas les droits ?
						//supprimer le fichier temp aussi ?
					}
				}
			}
		}
		if (redir->token == HEREDOC || redir->token == INPUT) // delink le heredoc a chaque fois, la gestion a -3 ne marche plus,
		{
			if ((node->fd_in == ERROR_OPEN || node->fd_out == ERROR_OPEN) && redir->token == HEREDOC) //cmd a ne pas lancer car au moins une redir invalide mais on lance les heredoc
			{
				node->fd_in = ft_open_heredoc(redir->name, node->here); //il faut le fermer ensuite
				node->here = HERE;
			}
			else if (node->fd_in != ERROR_OPEN && node->fd_out != ERROR_OPEN) // il n'y a pas eu de redir invalide pour l'instant
			{
				if (node->fd_in != NO_TRY_OPEN) //ft_close pour verifier qu'on a les droits pour fermer et close ensuite?
				{
					close(node->fd_in);
					if(access(".here", F_OK) == 0)
						unlink(".here");
				}
				if (redir->token == HEREDOC)
				{
					node->fd_in = ft_open_heredoc(redir->name, node->here);
					node->here = HERE;
				}
				else
				{
					node->fd_in = open(redir->name, O_RDONLY, 0644);
					if (node->fd_in == ERROR_OPEN)
					{
						perror("Error");
						if (node->fd_out >= 0)
						{
							close(node->fd_in);
							node->fd_in = -3;
						}
					}
				}
			}
		}
		redir = redir->next;
	}
}


void	handle_redir(t_cmd **lst)
{
	t_cmd	*current;

	current = *lst;
	while (current)
	{
		ft_fd_open(current);
		current = current->next;
	}
}


void test_case_1(void *gc) {
    t_cmd *lst = NULL;
    char *arr[] = {"echo", "Hello", ">", "output.txt", NULL};

    // Parsing simulerait ici (en fait appeler `parsing` ou le code nécessaire pour initialiser `lst`)
    // On simule la création de la redirection
    t_cmd *cmd = gc_malloc(sizeof(t_cmd), gc);
    cmd->cmd = arr;
    cmd->redir = create_redir(arr, 2, 3, gc);
    cmd->fd_in = -2;
    cmd->fd_out = -2;
    cmd->next = NULL;
    lst = cmd;

    // Appeler la fonction qui gère l'ouverture des fichiers
    ft_fd_open(lst);

    // Vérification de l'ouverture du fichier
    if (lst->fd_out != -1) {
        close(lst->fd_out);
        printf("Output file closed\n");
    }
}

// Fonction de test 2
void test_case_2(void *gc) {
    t_cmd *lst = NULL;
    char *arr[] = {"echo", "World", ">>", "append.txt", NULL};

    // Parsing simulerait ici (en fait appeler `parsing` ou le code nécessaire pour initialiser `lst`)
    // On simule la création de la redirection
    t_cmd *cmd = gc_malloc(sizeof(t_cmd), gc);
    cmd->cmd = arr;
    cmd->redir = create_redir(arr, 2, 3, gc); // suppose un redirection >> (à adapter selon le token)
    cmd->fd_in = -2;
    cmd->fd_out = -2;
    cmd->next = NULL;
    lst = cmd;

    // Appeler la fonction qui gère l'ouverture des fichiers
    ft_fd_open(lst);

    // Vérification de l'ouverture du fichier
    if (lst->fd_out != -1) {
        close(lst->fd_out);
        printf("Output file closed\n");
    }
}

// int main(void) {
//     void *gc = NULL;  // Initialise la gestion de la mémoire

//     // Appel des fonctions de test
//     test_case_1(gc);
//     test_case_2(gc);

//     return 0;
// }
