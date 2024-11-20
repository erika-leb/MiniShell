/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingv2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:53:57 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/19 20:03:30 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_file	*ft_lstnew_file(char *name, char token)
{
	t_file	*new;

	new = malloc(sizeof(t_cmd));
	if (new == NULL)
	{
		perror("malloc failed");
		return (NULL); // ou exit a voir plus tard
	}
	new -> name = name;
	new -> token = token;
	new -> next = NULL;
	return (new);
}

int	ft_classification(char **tab, int i)
{
	if (ft_strcmp(tab[i], "|") == 0)
		return (PIPE);
	if (i > 0 &&  ft_strcmp(tab[i - 1], "<") == 0)
		return (INPUT);
	if (i > 0 &&  ft_strcmp(tab[i - 1], "<<") == 0)
		return (HEREDOC);
	if (i > 0 &&  ft_strcmp(tab[i - 1], ">") == 0)
		return (TRUNC);
	if (i > 0 &&  ft_strcmp(tab[i - 1], ">>") == 0)
		return (APPEND);
	return (0); // pour linstqnt	 
}

void	ft_lstadd_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*current;

	if (!new)
		return ;
	if (!lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current -> next != NULL)
		current = current -> next;
	current -> next = new;
}

t_file	**create_redir(char **tab, int i, int last_i)
{
	t_file	**redir;
	t_file	*new;
	int		token;
	t_cmd	*current;
	
	i++;
	redir = NULL;
	while (i < last_i)
	{
		if (ft_strcmp(tab[i - 1], "<" == 0) || ft_strcmp(tab[i - 1], "<<" == 0) || ft_strcmp(tab[i - 1], ">" == 0) || ft_strcmp(tab[i - 1], ">>" == 0))
		{
			token = ft_classification(tab, i);
			ft_lstnew_file(tab[i], token);
			if (!redir)
				*redir = new;
			else
			{
				current = *redir;
				while (current -> next != NULL)
					current = current -> next;
				current -> next = new;
			}
		}
		i++;
	}
	return (redir);
}

void	create_chain(char **tab, int i, int last_i, t_cmd **lst)
{
	t_cmd	*new;
	t_cmd	*current;

	new = malloc(sizeof(t_cmd));
	if (new == NULL)
	{
		perror("malloc failed"); // clean tout et exit ici ou return pour exit apres
		exit(-1);
	}
	new->cmd = create_cmd(tab, i, last_i);
	new->redir = create_redir(tab, i, last_i);
	new->fd_in = -1;
	new->fd_out = -1;
	new->next = NULL;
	if (!lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current -> next != NULL)
		current = current -> next;
	current -> next = new;
}

void	parsing(char **tab, t_cmd **lst)
{
	int	i;
	int	last_i;

	i = 0;
	last_i = 0;
	while (tab[i])
	{
		if (ft_strcmp(tab[i], "|") != 0)
		{
			create_chain(tab, i, last_i, lst);
			last_i = i;
		}
		i++;
	}
	create_chain(tab, i, last_i, lst);
}

int	main(void)
{
	t_cmd	*lst;
	int		i;

	lst = NULL;
	char *arr[] = {"<<", "d", "echo", "okay", "<", "b", "baby", "<", "a", "<<", "c", "almost", ">", "e", "sure", "|", "cat", "|", "ls", "ls", ">>", "j", NULL};
	parsing(arr, &lst);
	
	
	t_cmd *current = lst;
    
	i=0;
    // while (current != NULL)
    // {
    //     printf("Name: %s, Token: %d\n", current->name, current->token);
    //     current = current->next;  // Avance au nœud suivant
	// 	i++;
	// 	if (i > 10)
	// 		break;
    // }
	return (0);
}

// il faudra parcourir la liste de redirections, note si au moins une est ko mais aller jusqu'au bout pour traiter les heredoc
// c'est toujours la derniere redicrection aui est pris en compte
// lorsau'on parcours la liste pour voir si tous les docs sont ouvrables, on arrete d'ouvrir a partir de la premiere erreur
// rajouter erreur si le tableau d'Adrien termine ou commence par | ou < > >> << ou si on a | apres < << > >> 'parse error, 
// comprendre ce qu'il se passe quand on a une commande sans commande => undefined behaviou je dirais