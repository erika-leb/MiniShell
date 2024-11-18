/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:33:45 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/18 13:44:29 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_addtoken(char *result, const char *token, int *i, int *j)
{
	int	k;
	int	len;

	k = 0;
	while (token[k])
	{
		result[*j] = token[k];
		(*j)++;
		k++;
	}
	k = 0;
	len = ft_strlen(token);
	while (k < len - 1)
	{
		(*i)++;
		k++;
	}
}

static void	ft_dotok(char *result, char *av2, int *i, int *j)
{
	result[*j] = ' ';
	(*j)++;
	if (av2[*i] == '<' && av2[*i + 1] == '<')
		ft_addtoken(result, "<<", i, j);
	else if (av2[*i] == '>' && av2[*i + 1] == '>')
		ft_addtoken(result, ">>", i, j);
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







//UNIQUEMENT DANS LES doubles quotes, si j'ai $ alors j'expand la variable d'environnement,
//si elle n'existe pas alors "".

// void	ft_expand(const char *src, char *res, size_t *i, size_t *j)
// {
// 	//								CHECKER ?
// 	//Parcourir tout ce qui suit $, tant qu'on arrive pas a la fin de src, tant qu'on voit pas
// 	//d'ESPACE ou la fin de " (donc ' sont compris). On check qu'il y a que des alphabetique ou des _
	
// 	static const char *srcbis;
// 	char *resbis;
// 	size_t		jbis;

// 	jbis = *j;
	
// 	srcbis = src;
// 	resbis = res;
// 	(*i)++;
// }


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Tester le expand avec le main de chatGPT (voir TODO)
void ft_expand(const char *src, char *res, size_t *i, size_t *j)
{
    char varname[20000];  // Un buffer pour stocker le nom de la variable d'environnement
    size_t varlen = 0;

    (*i)++;  // On saute le '$'

    // Récupérer le nom de la variable (les caractères valides sont alphanumériques ou '_')
    while (src[*i] && (isalnum(src[*i]) || src[*i] == '_') && varlen < sizeof(varname) - 1) {
        varname[varlen++] = src[*i];
        (*i)++;
    }
    varname[varlen] = '\0';  // Terminer le nom de la variable

    // Récupérer la valeur de la variable d'environnement
    char *value = getenv(varname);

    if (value) {
        // Copier la valeur de la variable dans la chaîne résultante
        size_t k = 0;
        while (value[k]) {
            res[*j] = value[k];
            (*j)++;
            k++;
        }
    } else {
        // Si la variable n'existe pas, on peut choisir de ne rien mettre ou mettre une chaîne vide
        // Ici on ne met rien
		return ;//Useless si jamais on decide de ne rien mettre. Je peux alors supprimer le else
    }
}

//1 : tokenizer $ en ajoutan tun espace comme pour < > etc (dotok)
//2 : comprendre pourquoi expand zappe tout ce qui vient apres le $
//Gerer cas speciaux enonces par mail

char	*ft_expenv(const char *src)
{
    static char res[80000];//Arbitraire, doit etre au moins aussi grand que 70 000
	size_t 		i;
	size_t		j;

	i = 0;
	j = 0;
    if (!src)
        return (NULL);//useless ? Puisqu'une chaine envoyée en argument ne sera pas un pointeur NULL
    while (src[i])
	{
		if (src[i] == '$')
			ft_expand(src, res, &i, &j);
		else
			res[j++] = src[i++];
    }
    res[j] = '\0';
    return (res);
}

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
	return (ft_expenv(result));
}