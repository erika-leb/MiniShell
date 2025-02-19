/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:03:56 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/17 12:22:58 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_copy_all( char **env, t_element *elements, t_gc *gc)
{
	int	i;
	int	s;

	i = 0;
	while (env[i])
	{
		s = ft_strlen(env[i]);
		elements->env[i] = gc_malloc(s + 1, gc);
		elements->env[i] = env[i];
		i++;
	}
	elements->env[i] = NULL;
}

void	ft_cpy_env(t_element *elements, char **env, t_gc *gc, char **argv)
{
	int		s_arr;
	int		i;

	if (env[0] == NULL)
		s_arr = 4;
	else
		s_arr = ft_arr_size(env);
	elements->env = gc_malloc(sizeof(char *) * (s_arr + 1), gc);
	i = 0;
	if (env[0] == NULL)
	{
		elements->env[0] = ft_strdup_("OLDPWD", gc);
		elements->env[1] = ft_strjoin_("PWD=", getcwd(NULL, 0), gc);
		elements->env[2] = ft_strdup_("SHLVL=1", gc);
		elements->env[3] = ft_strjoin_("_=", argv[0], gc);
		elements->env[4] = NULL;
	}
	else
	{
		ft_copy_all(env, elements, gc);
	}
}

t_element	*ft_init_struct(t_gc *gc)
{
	t_element	*elements;

	elements = NULL;
	elements = gc_malloc(sizeof(t_element), gc);
	elements->line = NULL;
	elements->arr = NULL;
	elements->lst = NULL;
	elements->mypaths = NULL;
	elements->nb_cmd = 0;
	elements->pid_arr = NULL;
	elements->pipes = NULL;
	elements->child_to_wait = 0;
	elements->error = NULL;
	elements->env = NULL;
	elements->exit_status = ft_itoa(0, gc);
	elements->line_valid = TRUE;
	return (elements);
}

char	*get_input(t_gc *gc)
{
	char	*input;
	char	buffer[1024];
	ssize_t	bytes_read;
	int		len;

	if (isatty(STDIN_FILENO))
	{
		input = readline("minishell> ");
		if (input && *input)
			add_history(input);
	}
	else
	{
		bytes_read = read(STDIN_FILENO, buffer, 1023);
		if (bytes_read <= 0)
			return (NULL);
		buffer[bytes_read] = '\0';
		len = 0;
		while (buffer[len] && buffer[len] != '\n')
			len++;
		buffer[len] = '\0';
		input = ft_strdup(buffer, gc);
	}
	return (input);
}
