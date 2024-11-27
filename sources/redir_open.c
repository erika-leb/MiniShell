/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_open.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:14:15 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/27 17:33:54 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_fd_open(t_cmd *node)
{
	t_file	*redir;

	redir = node->redir;
	while(redir)
	{
		if (redir->token == TRUNC || redir->token == APPEND)
		{
			if (node->fd_out != -1)
			{
				if (node->fd_out != -2)
					close(node->fd_out);
				if (redir->token == TRUNC)
					node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				else
					node->fd_out = open(redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (node->fd_out == -1)
					perror("Error");	
			}
		}
		if (redir->token == HEREDOC || redir->token == INPUT) // gerer le cas ou -1
		{
			if (node->fd_in == -1 && redir->token == HEREDOC)
			{
				printf("\n");
				//appliquer le heredoc
			}
			else
			{
				if (node->fd_in != -2 && node->fd_in != -3)
						close(node->fd_in);
				if (redir->token == HEREDOC)
				{
					// appliquer le heredor'
					node->fd_in = -3;
				}
				else
				{
					node->fd_in = open(redir->name, O_RDONLY, 0644);
					if (node->fd_in == -1)
						perror("Error");
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