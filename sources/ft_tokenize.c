/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:33:45 by aisidore          #+#    #+#             */
/*   Updated: 2024/11/18 18:14:27 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//expand qui foire = rien
//expand qui foire dans "" (ex "$HOMEE") renvoie chaine vide

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

// void	ft_getenv(const char *av2, int start, char *result, int *j)
// {
// 	int		i;
// 	char	ret[20000];
// 	char	*envv;

// 	i = 0;
// 	while (av2[start + i] && av2[start + i] != ' ' && av2[start + i] != '$'
// 			&& av2[start + i] != '\'' && av2[start + i] != '\"'
// 			&& (av2[start + i] == '_' || ft_isalnum(av2[start + i])))
// 	{
// 		ret[i] = av2[start + i];
// 		i++;
// 	}
// 	ret[i] = '\0';
// 	envv = getenv(ret);
// 	if (!envv)
// 		return ;
// 	i = 0;
// 	while (envv[i])
// 	{
// 		result[*j] = envv[i];
// 		(*j)++;
// 		i++;
// 	}
// }


// static void	ft_addbigtoken(char *result, const char *av2, int *i, int *j)
// {
// 	int	k;

// 	k = 0;
// 	while (av2[*i + k] == '$')
// 	{
// 		//Il faut reparcourir toute la chaine pour savoir si on doit expand ou non
// 		//Ou alors on le fait dans ft_dotok
// 		k++;
// 		ft_getenv(av2, *i + k, result, j);
// 		while (av2[*i + k] && av2[*i + k] != ' ' && av2[*i + k] != '$')
// 			k++; // on avance le long de la variable expand pour continuer
// 		//printf("%s\n", result);
// 	}
// 	//HERE
// 	// while (av2[*i + k] && av2[*i + k] != ' ')
// 	// {
// 	// 	result[*i + k] = av2[*i + k];
// 	// 	k++;
// 	// }
// 	*i += k - 1;
// }

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
	ft_addspace(result, av2, i, j);
}


//Gerer cas speciaux enonces par mail

//Regarder ce que fait bash si je fais '"$USER"', "'$USER'" : ma fct gere bien ?
//Que pasa si j'obtiens $$$$$$$

//Faire une fonction qui parcourt la chaine et verifie s'il y a des token interdits (UN_TOKEN) ; ; \ etc

static void	ft_expand(char *result, int *k)
{
	char	tmp[20000];
	char	*envv;
	//char	tmp_c;
	//int	tmp_k;
	int		i;

	i = 0;
	//printf("\nremplissage str pour expand\n\n");
 	while (result[*k + i] && (result[*k + i] == '_' || ft_isalnum(result[*k + i])))
 	{
 		tmp[i] = result[*k + i];
		//printf("%s\n", tmp);
 		i++;
 	}
	tmp[i] = '\0';
 	envv = getenv(tmp);
	printf("\na quoi ressemble envv : \n\n");
	if (!envv)
		return ;//Si on est dans une " alors chaine vide "" et sinon rien ?
	printf("%s\n", envv);
 	i = 0;
	printf("\nresult final :\n");
	while (tmp[i])
 	{//while (envv[i] && result[*k] == tmp[i])                result[*k]
		//Je parcours result et j'ecrase toute le nom de la var d'env
		if (envv[i] && result[*k] == tmp[i])
			result[*k] = envv[i];
		(*k)++;//On ecrase le nom de la var env
		i++;
 	}
	printf("%s\n", result);
	printf("envv + i : %s\n", envv + i);


	// while (result[*k] && envv[i])
	// {
	// 	tmp_c = result[*k];
	// 	result[*k] = envv[i];
	// 	result[*k + 1] = tmp_c;
	// 	(*k)++;//On ecrase le nom de la var env
	// 	i++;
	// }

 	// while (envv[i])
 	// {
	// 	result[*k] = envv[i];
	// 	printf("result : %s\n", result);
 	// 	(*k)++;
 	// 	i++;
 	// }
}

static void	ft_ifexpand(char *result)
{
	int	k;
	int	tmp;
	//int	sq;
	//Je pense qu'ici on peut supprimer les quote qui entourent le token ?

	k = 0;
	//Cette fonction doit servir a reperer les sq et double quote pour savoir si je dois expand ou pas
	//sq = 0;
	while (result[k])
	{
		//ft_sq(result, &sq); Si on est dans sq alors *sq = !(*sq).
		//car c'est jusqu'ici qu'il y avait un interet a les garder
		if (result[k] == '$')
		{
			tmp = k;//ecrase '$' en copiant/collant tous les elements a indice - 1
			//avant de lancer ft_expand (qui s'occupe de d'inserer la var env)
			while (result[tmp])
			{
				result[tmp] = result[tmp + 1];
				tmp++;
			}
			ft_expand(result, &k);
		}	
		k++;
	}
	result[k] = '\0';
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
	ft_ifexpand(result);
	return (result);
}