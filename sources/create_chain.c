/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_chain.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:56:56 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/27 16:46:27 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../gc/gc.h"

bool	ft_is_redir(char *s)
{
	if (ft_strcmp(s, "<") == 0 || ft_strcmp(s, "<<") == 0
		|| ft_strcmp(s, ">") == 0 || ft_strcmp(s, ">>") == 0)
		return (true);
	return (false);
}

bool	ft_is_str(char *s)
{
	if (ft_strcmp(s, "\n|") == 0)
		return (true);
	if (ft_strcmp(s, "\n<") == 0)
		return (true);
	if (ft_strcmp(s, "\n>") == 0)
		return (true);
	if (ft_strcmp(s, "\n<<") == 0)
		return (true);
	if (ft_strcmp(s, "\n>>") == 0)
		return (true);
	return (false);
}

void	create_chain(char **tab, int i, int last_i, t_cmd **lst, t_gc *gc)
{
	t_cmd	*new;
	t_cmd	*current;

	new = gc_malloc(sizeof(t_cmd), gc);
	//perror("test");
	// if (new == NULL)
	// {
	// 	perror("malloc failed"); // clean tout et exit ici ou return pour exit apres
	// 	exit(-1);
	// }
	new->cmd = NULL;
	new->redir = create_redir(tab, i, last_i, gc);
		//perror("test2");
	new->cmd = cmd_arr(tab, i, last_i, gc);
	//new->here = NO_HERE;
		//perror("test3");
	// int	j = 0;
	// while(new->cmd[j])
	// {
	// 	printf("ici tab[%d] = %s\n", j, new->cmd[j]);
	// 	j++;
	// }
	new->fd_in = -2;
	new->fd_out = -2;
	new->active = TRUE;
	new->next = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current -> next != NULL)
		current = current -> next;
	current -> next = new;
}
