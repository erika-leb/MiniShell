/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_functions_parta.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:32:21 by marvin            #+#    #+#             */
/*   Updated: 2024/11/17 22:00:21 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_lstnew(char *str, int token)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (new == NULL)
	{
		perror("malloc failed");
		//write(2, "Error memory allocation\n", 26);
		return (NULL);
	}
	new -> name = str;
	new -> token = token ;
	new -> next = NULL;
	return (new);
}

void	ft_lstadd_front(t_token **lst, t_token *new)
{
	new -> next = *lst;
	*lst = new;
}

int	ft_lstsize(t_token **lst)
{
	int		s;
	t_token	*current;

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
