/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:57:58 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/14 16:39:30 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	number_words(char *s, char c)
{
	int	i;
	int	r;

	i = 1;
	r = 0;
	if (ft_strlen(s) == 0)
		return (0);
	if (s[0] != c)
		r++;
	while (s[i])
	{
		if (s[i] != c && s[i - 1] == c)
			r++;
		i++;
	}
	return (r);
}

static int	write_word_path(char **str, char *s, t_var *var, t_gc *gc)
{
	int	i;

	i = 0;
	str[var->j] = gc_malloc(sizeof(char) * ((var->k) + 2), gc);
	while (i < var->k)
	{
		str[var->j][i] = s[i];
		i++;
	}
	str[var->j][i] = '/';
	str[var->j][i + 1] = '\0';
	return (0);
}

static char	**ft_split_path(char *s, char c, t_gc *gc)
{
	char	**str;
	char	*s1;
	t_var	*var;

	var = gc_malloc(sizeof(t_var), gc);
	var->j = number_words(s, c);
	var->k = 0;
	str = gc_malloc(sizeof(char *) * ((var->j) + 1), gc);
	(var->j) = 0;
	s1 = (char *) s;
	while (*s1)
	{
		(var->k) = 0;
		if ((s1 == s && s[0] != c) || (*s1 != c && *(s1 - 1) == c))
		{
			while (*(s1 + (var->k)) && *(s1 + (var->k)) != c)
				(var->k)++;
			if (write_word_path(str, s1, var, gc) == 1)
				return (NULL);
			(var->j)++;
		}
		s1++;
	}
	str[(var->j)] = NULL;
	return (str);
}

void	ft_handle_path(t_element *elements, t_gc *gc)
{
	char	*path_env;
	int		i;
	int		s;

	i = 0;
	path_env = NULL;
	while (elements->env[i] != NULL)
	{
		if (ft_strncmp(elements->env[i], "PATH=", 5) == 0)
		{
			s = ft_strlen(elements->env[i]);
			path_env = ft_substr(elements->env[i], 5, s - 5, gc);
		}
		i++;
	}
	if (path_env != NULL)
		elements->mypaths = ft_split_path(path_env, ':', gc);
}
