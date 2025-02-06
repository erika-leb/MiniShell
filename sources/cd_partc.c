/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_partc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 18:16:02 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/05 18:55:57 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd_error(t_built *built, t_gc *gc)
{
	//perror("et marina dans tout ca")
	ft_buff_error("minishell: cd: ", built->elements, gc);
	ft_buff_error(built->cmd[1], built->elements, gc);
	ft_buff_error(": ", built->elements, gc);
	ft_buff_error(strerror(errno), built->elements, gc);
	ft_buff_error("\n", built->elements, gc);
	ft_write_error(built->elements, gc);
}

void	remove_old_env(t_env *head, t_built *built, t_gc *gc)
{
	char	**new_env;

	new_env = ft_ltoa(head, gc);
	gc_remove(gc, built->elements->env);
	built->elements->env = new_env;
	gc_remove(gc, head);
}

void	change_var_name(t_env *current, const char *buff, t_gc *gc)
{
	char	*new_value;
	int		len;

	len = ft_strlen(buff);
	new_value = gc_malloc(len + 1, gc);
	ft_strncpy(new_value, buff, len);
	new_value[len] = '\0';
	gc_remove(gc, current->key);
	current->key = new_value;
}

int	is_home_set(char **env, t_gc *gc)
{
	t_env	*head;
	t_env	*current;
	int		i;

	i = -1;
	head = NULL;
	while (env[++i])
		ft_adder(&head, env[i], gc);
	current = head;
	while (current)
	{
		if (ft_strcmp(current->name, "HOME") == 0)
		{
			if (ft_strcmp(current->key, "") == 0)
				return (3);
			return (TRUE);
		}
		current = current->next;
	}
	return (FALSE);
}

int	cd_home(t_built *built, t_gc *gc)
{
	t_env	*head;
	t_env	*current;
	char	*str;
	int		i;

	i = -1;
	head = NULL;
	while (built->elements->env[++i])
		ft_adder(&head, built->elements->env[i], gc);
	current = head;
	while (current)
	{
		if (strcmp(current->name, "HOME") == 0)
		{
			str = current->key;
			if (chdir(str) != 0)
				return (1);
			else
				return (0);
		}
		current = current->next;
	}
	return (1);
}
