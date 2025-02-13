/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:57:18 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/13 18:02:08 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

char *ft_cut(const char *src, char delim, int is_end, t_gc *gc)
{
	char *result;
	size_t i;

	i = 0;
	while (src[i] && src[i] != delim)
		i++;
	if (is_end)
	{
		if (src[i] == delim)
			return (ft_strdup_(src + i + 1, gc));
		return (NULL);
	}
	else
	{
		result = gc_malloc(i + 1, gc);
		ft_strncpy(result, src, i);
		result[i] = '\0';
		return (result);
	}
}

void ft_printexport(const t_env *head, t_element *elements, t_gc *gc)
{
    const t_env *current = head;
    while (current)
	{
        if (current->key)
		{
			ft_buff_error("export ", elements, gc);
			ft_buff_error(current->name, elements, gc);
			ft_buff_error("=\"", elements, gc);
			ft_buff_error(current->key, elements, gc);
			ft_buff_error("\"\n", elements, gc);
			ft_write_mess(elements, gc);
		}
        else
		{
			ft_buff_error("export ", elements, gc);
			ft_buff_error(current->name, elements, gc);
			ft_buff_error("\n", elements, gc);
			ft_write_mess(elements, gc);
		}
		current = current->next;
    }
}

void ft_adder(t_env **head, char *str, t_gc *gc)
{
	char  **adder;

	adder = gc_calloc(2 + 1, sizeof(char *), gc);
	adder[0] = ft_cut(str, '=', 0, gc);
	adder[1] = ft_cut(str, '=', 1, gc);
	*head = ft_addenvnode(*head, adder[0], adder[1], gc);
	gc_remove(gc, adder[0]);
	gc_remove(gc, adder[1]);
	gc_remove(gc, adder);
}

int	ft_dr(t_exp *xx, t_env *head, t_element *element, t_gc *gc)
{
	if (xx->argv[1] == NULL)
	{
		(ft_bbsort(head), ft_printexport(head, element, gc));
		gc_remove(gc, head);
		if (xx->ch == 0)
		{
			element->exit_status = ft_itoa(0, gc);
			return (0);
		}
		else
			(gc_cleanup(gc), free_std(), exit(0));
	}
	return (1);
}

int	ft_opp(t_exp *xx, t_env *head, t_element *element, t_gc *gc)
{
	if (xx->argv[1][0] == '-')
	{
		write(2, "minishell: export: No option(s) allowed\n", 41);
		gc_remove(gc, head);
		if (xx->ch == 0)
		{
			element->exit_status = ft_itoa(2, gc);
			return (0);
		}
		else
			(gc_cleanup(gc), free_std(), exit(2));
	}
	return (1);
}