/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/20 18:48:02 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static int ft_checkq(char *input)
{
	int		sq;
	int		dq;
	char	*ptr;

	sq = 0;
	dq = 0;
	ptr = input;
	while (*ptr)
	{
		if (!sq && !dq && *ptr == '$' && *(ptr + 1) == '$')
			return(printf("%s", UN_DOLLAR));//write mais useless
		if (*ptr == '\'' && !dq)
			sq = !sq;
		else if (*ptr == '\"' && !sq)
			dq = !dq;
		ptr++;
	}
	if (sq || dq)
		return(printf("minishell: unclosed quotes\n"));//write mais useless
	return (0);
}

int	ft_istok_(char *av2)
{
	if (!ft_strcmp(av2, "|") || !ft_strcmp(av2, "<") || !ft_strcmp(av2, ">")
		|| !ft_strcmp(av2, "<<") || !ft_strcmp(av2, ">>"))
		return (1);
	return (0);
}

static int	ft_unexptoken(char **result)
{
	int	i;

	//JE COMPREND RIEN:
	//\0 est un unexpected token
	if (!(*result))
		return (1);//Sinon ecrire juste $a (qui ne correspond a rien) provoquait un segfault a cause de ft_strcmp
	if (!ft_strcmp(result[0], "|"))
		return(printf("%s", UN_PIPE));
	i = 0;
	while (result[i])
	{
		if (!ft_strcmp(result[i], "|")
			&& result[i + 1] && !ft_strcmp(result[i + 1], "|"))
			return(printf("%s", UN_PIPE));
		if (ft_istok_(result[i]) && ft_strcmp(result[i], "|")
			&& result[i + 1] && ft_istok_(result[i + 1]))
		{
			printf("minishell: syntax error near unexpected token `");
			return(printf("%s'\n", result[i + 1]));
		}
		if (ft_istok_(result[i]) && !result[i + 1])
			return(printf("%s", UN_NLINE));
		i++;
	}
	return (0);
}

void	ft_deldollar(char *input)
{
	int	sq;
	int	dq;
	int	i;

	sq = 0;
	dq = 0;
	i = 0;
	while (input[i])
	{
		ft_modifquote_(input, &sq, &dq, &i);
		if (!sq && !dq && input[i] == '$'
			&& (input[i + 1] == '\'' || input[i + 1] == '\"'))
			ft_erase(input, i);
		i++;
	}
}

//Rechecker la logique de ce truc.
void	ft_ft(t_element *elements, t_gc *gc)
{
	int	i;
	int	go;

	i = -1;
	go = 0;
	if (!ft_checkq(elements->line))
		go = 1;
	else
		return ;
	//Doit on transformer les static char 70 000 en malloc ?
	elements->arr = ft_split(ft_tokenize(elements->line, gc), 0, 0, gc);
	while (go && elements->arr && elements->arr[++i])
		elements->arr[i] = ft_concat(elements->arr[i], -1, 0, 0);
	if (elements->arr && ft_unexptoken(elements->arr))
	{
		//Il faut que j'envoie a Erika que les heredoc : << + nom delim
		//Il faut que je lui envoie un
		gc_remove(gc, elements->arr);
		elements->arr = NULL;
		return ;
	}
	i = -1;
	while (go && elements->arr && elements->arr[++i])
		printf("token %d :%s\n", i, elements->arr[i]);//juste pour checker, useless sinon
}
