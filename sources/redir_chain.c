/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_chain.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:11:46 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/26 18:30:06 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_file	*ft_lstnew_file(char *name, char token, t_gc *gc)
{
	t_file	*new;

	//perror("salut");
	new = gc_malloc(sizeof(t_cmd), gc);
	// if (new == NULL)
	// {
	// 	perror("malloc failed");
	// 	return (NULL); // ou exit a voir plus tard
	// }
	new -> name = name;
	new -> token = token;
	new -> next = NULL;
	//printf("new->name = %s token =%d\n", new->name, new->token );
	return (new);
}

static int	ft_classification(char **tab, int i)
{
	//perror("ult");
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

void	ft_lstadd_back(t_file **lst, t_file *new)
{
	t_file	*current;

	if (!new)
		return ;
	if (!lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	//printf("ICI next = %p \n", new-> next);
	while (current -> next != NULL)
		current = current -> next;
	current -> next = new;
	//printf("LA next = %p \n", new-> next);
}

t_file	*create_redir(char **tab, int i, int last_i, t_gc *gc)
{
	t_file	*redir;
	t_file	*new;
	int		token;
//	t_file	*current;

	i++;
	// perror("ici");
	redir = NULL;
	//perror("ici");
	//printf("i = %d, last-i =%d\n", i, last_i);
	//printf("tab[i - 1] = %s\n", tab[i - 1]);
	while (last_i < i && tab[i - 1])
	{
	//	perror("la");
	//	printf("i -1 = %d\n", last_i - 1);
		// printf("i -1 = %d\n", last_i - 1);
		if (last_i > 0 && ft_is_redir(tab[last_i - 1]) == true) // segfault ici a cause du - 1
		{
			//perror("c moyen cool");
			token = ft_classification(tab, last_i);
			//perror("c cool");
			new = ft_lstnew_file(tab[last_i], token, gc);
			//printf("new->name = %s token =%d, next = %p \n", new->name, new->token, new->next);
			// if (new == NULL)
			// 	exit(-1);
			if (!redir)
			{
				//perror("hola pri;ero");
				redir = new;
				//printf(" ICI next = %p \n", (*redir)->next);
			}
			else
				ft_lstadd_back(&redir, new);
		}
		//perror("cpa cool");
		last_i++;
	}
	//print_redir(redir);
	return (redir);
}

