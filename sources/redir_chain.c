/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_chain.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:11:46 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/28 21:49:20 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_file	*ft_lstnew_file(char *name, char token, t_gc *gc)
{
	t_file	*new;

	new = gc_malloc(sizeof(t_cmd), gc);
	// if (new == NULL)
	// {
	// 	perror("malloc failed");
	// 	return (NULL); // ou exit a voir plus tard
	// }
	new -> name = name;
	new -> token = token;
	new -> next = NULL;
	return (new);
}

static int	ft_classification(char **tab, int i)
{
	if (ft_strcmp(tab[i], "|") == 0)
		return (PIPE);
	if (i > 0 && ft_strcmp(tab[i - 1], "<") == 0)
		return (INPUT);
	if (i > 0 && ft_strcmp(tab[i - 1], "<<") == 0)
		return (HEREDOC);
	if (i > 0 && ft_strcmp(tab[i - 1], ">") == 0)
		return (TRUNC);
	if (i > 0 && ft_strcmp(tab[i - 1], ">>") == 0)
		return (APPEND);
	return (0);
}

static void	ft_lstadd_back(t_file **lst, t_file *new)
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
	while (current -> next != NULL)
		current = current -> next;
	current -> next = new;
}

t_file	*create_redir(char **tab, int i, int last_i, t_gc *gc)
{
	t_file	*redir;
	t_file	*new;
	int		token;

	i++;
	redir = NULL;
	while (last_i < i && tab[i - 1] != NULL)// attention ici il y a peut etre un leak,
	{
		if (last_i > 0 && ft_is_redir(tab[last_i - 1]) == true)
		{
			token = ft_classification(tab, last_i);
			new = ft_lstnew_file(tab[last_i], token, gc);
			if (!redir)
				redir = new;
			else
				ft_lstadd_back(&redir, new);
		}
		last_i++;
	}
	return (redir);
}
