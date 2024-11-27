/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:33:45 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/25 17:20:13 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//expand qui foire = rien
//expand qui foire dans "" (ex "$HOMEE") renvoie chaine vide

//ATTENTION si y'a une boucle infinie ca bug je crois : on peut plus utiliser minishell apres avoir
//forcé l'arret de la boucle

//ATTENTION il faut free split si y'a un pb !!!

#include "minishell.h"

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

static void	ft_addspace(char *result, char *av2, int *i, int *j)
{
	if ( *i != 0 && av2[*i] != ' ' && ft_istok(av2 + *i))
	{
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
		ft_modifquote(av2, &sq, &dq, &k);
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


//Gerer cas speciaux enonces par mail

//Que pasa si j'ecris $$$$$$$ : ca le prend en entier comme un token, alors que sur bash ca sort
//le PID actuel du processus pour chaque paire de $$. Donc une concatenation du meme PID.

//Faire une fonction qui parcourt la chaine et verifie s'il y a des token interdits (UN_TOKEN) ; ; \ hors des chaines
//donc c'est a faire avant de concat

//Gerer les <<<<<< ou ||||| dans le 1er parsing (avant tokenization)

 //minishell> 'ofjoifjos '''fsofjsd
//token 0 :'ofjoifjos 'fsofjsd alors qu'on devrait avoir 'ofjoifjos 'fsofjsd




char	*ft_tokenize(char *av2)
{
	static char	result[70000];
	int			i;
	int			j;

	//Il faut s'assurer que les token qui ressortent comme "" sont bien consideres comme des chaines vides
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
	//'$HOME'a   '$HOME''a'   '$HOME'"a" deviennent $HOMEa : faire les etapes du TODO pour faire une concatenation correcte
	return (ft_ifexpand(result));
	//return (ft_concat(ft_ifexpand(result), 0, 0));
	//return (ft_concat(ft_concat(ft_ifexpand(result), 0, 0), 0, 0));
}