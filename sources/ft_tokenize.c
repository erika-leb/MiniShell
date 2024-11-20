/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:33:45 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/20 19:37:11 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_addtoken(char *result, const char *token, int *i, int *j)
{
	int	k;

	k = 0;
	while (token[k])
	{
		result[*j] = token[k];
		(*j)++;
		k++;
	}
	*i += k - 1;
}

void	ft_trunc(const char *av2, int start, char *result, int j)
{
	int	i;
	static char ret[20000];

	i = 0;
	while (av2[start + i] && av2[start + i] != ' ' && av2[start + i] != '$')
	{
		ret[i] = av2[start + i];
		i++;
	}
	ret[i] = '\0';

	static char *envv;
	i = 0;
	envv = getenv(ret);
	printf("%s\n", envv);
	while (envv[i])
	{
		result[j + i] = envv[i];
		i++;
	}
}

static void	ft_addbigtoken(char *result, const char *av2, int *i, int *j)
{
	// int	k;
	
	// k = 0;
	// while (av2[*i + k] != ' ')
	// {
	// 	result[*j] = av2[*i + k];
	// 	(*j)++;
	// 	k++;
	// }
	// *i += k - 1;

	
	int	k = 0;
	
	while (av2[*i + k] != ' ')
	{
		if (av2[*i + k] == '$')
		{
			k++;
			ft_trunc(av2, *i + k, result, *j);
			break ;//Qu'il y ait break ou pas ca passe pr l'instant
		}
		//result[*j] = av2[*i + k];
		//(*j)++;
		//k++;
	}
	*i += k - 1;
}

//Parcourir chaine, si $ alors copier dans res getenv(str + 1)
//jusqu'a sep ou jusqu'a $

static void	ft_dotok(char *result, char *av2, int *i, int *j)
{
	result[*j] = ' ';
	(*j)++;
	if (av2[*i] == '<' && av2[*i + 1] == '<')
		ft_addtoken(result, "<<", i, j);
	else if (av2[*i] == '>' && av2[*i + 1] == '>')
		ft_addtoken(result, ">>", i, j);
	else if (av2[*i] == '$')
		ft_addbigtoken(result, av2, i, j);
	else
	{
		result[*j] = av2[*i];
		(*j)++;
	}
	result[*j] = ' ';
	(*j)++;
}

static int	ft_istok(char *av2)
{
	if (*av2 == '|' || *av2 == '<'
		|| *av2 == '>' || *av2 == '$')
		return (1);
	return (0);
}


//coucou


//////////////// ft_expand et/ou ft_expenv marchent pas

//UNIQUEMENT DANS LES doubles quotes, si j'ai $ alors j'expand la variable d'environnement,
//si elle n'existe pas alors "".




//Gerer cas speciaux enonces par mail
//Regarder ce que fait bash si je fais '"$USER"', "'$USER'" : ma fct gere bien ?
//Aue pasa si j'obtiens $$$$$$$


char	*ft_tokenize(char *av2)
{
	static char	result[70000];
	int			i;
	int			j;

	i = 0;
	j = 0;
	while (av2[i])
	{
		if (ft_istok(av2 + i))
			ft_dotok(result, av2, &i, &j);
		else
			result[j++] = av2[i];
		i++;
	}
	result[j] = '\0';
	//return (ft_expenv(result));
	return (result);
}