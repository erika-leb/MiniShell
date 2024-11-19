/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingv2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:53:57 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/19 19:06:03 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*ft_lstnew(char *in, char *out, char **cmd)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (new == NULL)
	{
		perror("malloc failed");
		//write(2, "Error memory allocation\n", 26);
		return (NULL);
	}
	new -> in = in;
	new -> out = out;
	new -> cmd = cmd;
	new -> next = NULL;
	return (new);
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

t_cmd	*create_chain(char **tab, int i, int last_i, t_cmd **lst)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (new == NULL)
	{
		perror("malloc failed"); // clean tout et exit ici ou return pour exit apres
		exit(-1);
	}
	
}

void	parsing(char **tab, t_cmd **lst) //rajouter erreur si le tableau d'Adrien termine par | ou < > >> <<
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