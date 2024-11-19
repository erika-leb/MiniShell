/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_functions_partb.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 09:32:21 by marvin            #+#    #+#             */
/*   Updated: 2024/11/18 17:25:23 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_lstlast(t_token *lst)
{
	t_token	*current;

	if (lst == NULL)
		return (NULL);
	current = lst;
	printf("cfcdcf\n");
	while (current -> next != NULL)
	{
			printf("cf\n");
		current = current -> next;
			printf("cfff\n");
	}
	printf("cff\n");
	return (current);
}

void	ft_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*current;

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

void	ft_lstclear(t_token *lst)
{
	t_token	*current;

	while (lst != NULL)
	{
		current = (lst)-> next;
		free(lst);
		lst = current;
	}
	lst = NULL;
}
