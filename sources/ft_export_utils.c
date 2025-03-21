/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/14 12:52:29 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

t_env	*ft_envnode(char *name, char *key, t_gc *gc)
{
	t_env	*new_node;

	new_node = gc_malloc(sizeof(t_env), gc);
	new_node->name = ft_strdup_(name, gc);
	if (key)
	{
		new_node->key = ft_strdup_(key, gc);
		gc_remove(gc, key);
	}
	else
		new_node->key = NULL;
	new_node->next = NULL;
	return (new_node);
}

static void	ft_addkey(int catt, t_env *current, char *key, t_gc *gc)
{
	if (key)
	{
		if (catt)
			current->key = ft_strjoin_(current->key, key, gc);
		else
		{
			gc_remove(gc, current->key);
			current->key = ft_strdup_(key, gc);
		}
	}
}

t_env	*ft_addenvnode(t_env *head, char *name, char *key, t_gc *gc)
{
	t_env	*current;
	int		catt;

	catt = 0;
	if (name[ft_strlen(name) - 1] == '+')
	{
		ft_erase(name, ft_strlen(name) - 1);
		catt = 1;
	}
	if (!head)
		return (ft_envnode(name, key, gc));
	current = head;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			ft_addkey(catt, current, key, gc);
			return (head);
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	current->next = ft_envnode(name, key, gc);
	return (head);
}

void	ft_swapnodes(t_env *node1, t_env *node2)
{
	char	*temp_name;
	char	*temp_key;

	temp_name = node1->name;
	temp_key = node1->key;
	node1->name = node2->name;
	node1->key = node2->key;
	node2->name = temp_name;
	node2->key = temp_key;
}

void	ft_bbsort(t_env *head)
{
	int		swapped;
	t_env	*ptr1;
	t_env	*lptr;

	if (head == NULL)
		return ;
	swapped = 1;
	lptr = NULL;
	while (swapped)
	{
		swapped = 0;
		ptr1 = head;
		while (ptr1->next != lptr)
		{
			if (ft_strcmp(ptr1->name, ptr1->next->name) > 0)
			{
				ft_swapnodes(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}
}
