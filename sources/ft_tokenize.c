/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:33:45 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/11 17:53:12 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//ATTENTION il faut free split si y'a un pb !!!

#include "../minishell.h"

static int	ft_istok(char *av2)
{
	if (*av2 == '|' || *av2 == '<'
		|| *av2 == '>')
		return (1);
	return (0);
}

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

//*i != 0 me gene pour les cas >'' ou tout autre token en position 0 colle a quelque chose
//mais j'ai peur d'y toucher. Je pensequ'il etait là car ca me genait de mettre un espace au debut
//de result de facon inutile. Meme le av2[*i] != ' ' me semble useless car ft_istok contraint le caractere
//a etre uniquement un token.
//Le pb etait que ma tokenization n'ajoute pas d'espaces quand j'ecris <'' et |''. Pour le reste ca marche
//Comme ils sont en premiere position, < et | n'enclenchent pas ft_addspace
static void	ft_addspace(char *result, char *av2, int *i, int *j)
{
	if (ft_istok(av2 + *i))
	{//*i != 0 && av2[*i] != ' ' && 
		result[*j] = ' ';
		(*j)++;
	}
}

static void	ft_dotok(char *result, char *av2, int *i, int *j)
{
	int	k;
	int	sq;
	int	dq;
	
	k = -1;
	sq = 0;
    dq = 0;
	//Pas d'espace au niveau d'un token si on est a l'interieur d'une chaine
	while (++k <= *i)
		ft_modifquote_(av2, &sq, &dq, &k);
	if (!sq && !dq)
		ft_addspace(result, av2, i, j);
	if (av2[*i] == '<' && av2[*i + 1] == '<')
		ft_addtoken(result, "<<", i, j);
	else if (av2[*i] == '>' && av2[*i + 1] == '>')
		ft_addtoken(result, ">>", i, j);
	else
	{
		result[*j] = av2[*i];
		(*j)++;
	}
	if (!sq && !dq)
		ft_addspace(result, av2, i, j);
}

char	*ft_tokenize(char *av2)
{
	static char	result[70000];
	int			i;
	int			j;

	i = 0;
	j = 0;
	ft_deldollar(av2);//voir parsing.c
	while (av2[i])
	{
		if (ft_istok(av2 + i))
			ft_dotok(result, av2, &i, &j);
		else
			result[j++] = av2[i];
		i++;
	}
	result[j] = '\0';
	//ft_tokenize sert a separer les tokens par des espaces dans la chaine av2.
	//Par ex <<'oui'|'non' envoie dans ft_ifexpand : << 'oui' | 'non'

	return (ft_ifexpand(result, 0, 0));
}