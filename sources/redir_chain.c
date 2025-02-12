/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_chain.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 16:11:46 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/12 16:51:28 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		ft_bst(char *name)
{
	int	i;

	i = -1;
	while (name[++i])
	{
		if (name[i] == '\t')
			return (1);
	}
	return (0);
}

char		*ft_filter(char *name, t_element *elements, t_gc *gc)
{
	static char	res[20000];
	int		i;

	if (!ft_bst(name))
		return (name);
	ft_strncpy(res, name, ft_strlen(name));
	res[ft_strlen(name)] = '\0';
	i = -1;
	while (res[++i])
	{
		if (res[i] == '\t' && res[i - 1] == '$')
		{
			ft_erase(res, i);
			ft_hereifexpand(res, elements, gc);
		}
	}
	i = -1;
	while (res[++i])
	{
		if (res[i] == '\t')
			ft_erase(res, i);
	}
	return (res);
}

static t_file	*ft_lstnew_file(char *name, char token, t_element *elements, t_gc *gc)
{
	t_file	*new;

	new = gc_malloc(sizeof(t_cmd), gc);
	new -> name = ft_filter(name, elements, gc);
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

t_file	*create_redir(char **tab, int i, int last_i, t_element *elements, t_gc *gc)
{
	t_file	*redir;
	t_file	*new;
	int		token;

	i++;
	redir = NULL;
	while (last_i < i && tab[i - 1] != NULL)
	{
		if (last_i > 0 && ft_is_redir(tab[last_i - 1]) == true)
		{
			token = ft_classification(tab, last_i);
			new = ft_lstnew_file(tab[last_i], token, elements, gc);
			if (!redir)
				redir = new;
			else
				ft_lstadd_back(&redir, new);
		}
		last_i++;
	}
	return (redir);
}
