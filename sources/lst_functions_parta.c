/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_functions_parta.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:32:21 by marvin            #+#    #+#             */
/*   Updated: 2024/11/19 18:57:50 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	ft_lstadd_front(t_cmd **lst, t_cmd *new)
{
	new -> next = *lst;
	*lst = new;
}

int	ft_lstsize(t_cmd **lst)
{
	int		s;
	t_cmd	*current;

	s = 0;
	current = *lst;
	if (*lst == NULL)
		return (0);
	while (current != NULL)
	{
		s++;
		current = current -> next;
	}
	return (s);
}
