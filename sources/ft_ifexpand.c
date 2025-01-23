/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ifexpand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/23 16:31:05 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"


// //Fonction qui retourne 1 si y'a des espaces, 2 si y'a des quotes et 0 sinon.
// static int	ft_spacequotes(char	*envv)
// {
// 	int	i;

// 	i = 0;
// 	while (envv[i])
// 	{
// 		if (envv[i] == ' ')
// 			return (1);
// 		if (envv[i] == '\'' || envv[i] == '\"')
// 			return (2);
// 		i++;
// 	}
// 	return (0);
// }

static void	ft_expand(char *result, int *k, t_element *elements)
{
	char	tmp[20000];
	char	*envv;
	int		i;
	int		start;

	start = *k;
    envv = ft_getenvv(result, k, tmp, elements);//si y'a des quotes alors j'expand pas et envv prend la valeur de $\n\t ...
    if (!envv)
        return (ft_erase_substr(result, k, tmp));
	//Si envv contient des quotes (qu'il soit full ou pas on s'en fiche) alors on n'expand pas et on
	//remet le dollar + /n/t. Je n'oublie pas d'ecraser tmp comme fait juste en dessous.
	//ft_hedgecase va inserer $ + /n/t et incrementer k de ft_strlen(tmp) + 3. Ensuite on se sert d'une fonction
	//qui ressemble a ft_fatalerror pour expand, et resize un new_array avec une chaine de la bonne taille et eventuellement
	//split (sep = ' ') si y'avait des espaces en + des quotes.
	// if (ft_spacequotes(envv) == 2)
		// return (ft_hedgecase(result, k, tmp));
    i = 0;
	while (tmp[i])
 	{
		if (envv[i] && result[*k] == tmp[i])
			result[*k] = envv[i];
		(*k)++;
		i++;
 	}
	while (envv[i])
	{
		ft_insert(result, (*k), envv[i]);
		(*k)++;
		i++;
	}
	(*k)--;
}

static void	ft_delim(char *result, int *k, int sq, int dq)
{
	*k += 2;
	while (result[*k] == ' ')
		(*k)++;
	ft_modifquote_(result, &sq, &dq, k);
	if (sq)
	{
		(*k)++;
		while (result[*k] && !(result[*k] == '\''
				&& result[*k + 1] == ' '))
			(*k)++;
	}
	else if (dq)
	{
		(*k)++;
		while (result[*k] && !(result[*k] == '\"'
				&& result[*k + 1] == ' '))
			(*k)++;
	}
	else
	{
		while (result[*k] && result[*k] != ' ')
			(*k)++;
	}
}

static void	ft_incrk(char *result, int *k)
{
	(*k)++;
	if (!ft_strncmp(result + *k, "> ", 2))
		(*k)++;
	while (result[*k] == ' ')
		(*k)++;
}

char	*ft_ifexpand(char *result, int sq, int dq, t_element *elements)
{
	int	k;

	k = 0;
	while (result[k])
	{
		ft_modifquote_(result, &sq, &dq, &k);
		//Si je suis hors des quotes (sq et dq) et que ft_strncmp(result + k, 3, "<< ") c'est que j'ai repéré un heredoc
		//qui a ete prealablement separé par ft_tokenize. J'avance de +2, je me situe donc forcement
		//sur un espace normalement. Il ne faut pas expand ce qui vient apres.
		//et donc je modifie aussi le if pour ne pas qu'il s'enclenche.
		if (!sq && !dq && !ft_strncmp(result + k, "<< ", 3))
			ft_delim(result, &k, 0, 0);//On est forcement hors quote donc sq = 0 et dq = 0 en param

		//ambiguous redirect ne fonctionne que si $a n'est pas dans une quote. Car si sq on retire juste les quote
		//et on a pas a expand. Si dq on renvoie une chaine vide.
		//ls < $HOME < $b entraine ambiguous redirect meme si le 1er est valable.
		if (!sq && !dq && (!ft_strncmp(result + k, ">> ", 3)
			|| !ft_strncmp(result + k, "> ", 2)
			|| !ft_strncmp(result + k, "< ", 2)))
		{
			ft_incrk(result, &k);
			ft_modifquote_(result, &sq, &dq, &k);//soit on est sur une quote soit on est sur autre chose
			//si on est sur une quote on change juste la valeur de sq et dq et on laisse ifexpand faire son travail

			//Mais avant de laisser ifexpand faire son travail, on check que si y'a des quotes ou des espaces dans la var d'env.
			//Si c'est le cas on ajoute un \n\t et on fait un peu comme ft_delim en incremantant k pour parcourir/zapper le nom
			//de la var d'env.
			// if (ft_spacequotes(envv))
				// return (ft_hedgecase(result, k, tmp));

			//Je ne peux pas utiliser ft_ambig car il ne s'applique pas si je suis en double quote et mine de rien je peux avoir des cas ou
			//le user fais export a="  "haha" 'ca va bien' " auquel cas j'a interet a aussi gerer le cas ou je suis dans des doubles quotes.
			if (!sq && !dq)
				ft_ambig(result + k, &k, elements);
		}
		//S'assurer qu'Erika n'a pas mis $ comme token, comme ca si je lui envoie $ c'est qu'elle doit le traiter comme sa valeur litterale.
		//ft_erase ecrase '$' en copiant/collant tous les elements a indice - 1, pour lancer ft_expand sur ce qui vient apres
		if (result[k] == '$' && !sq && (result[k + 1] == '_'
			|| ft_isalnum(result[k + 1]) || result[k + 1] == '?'))
			ft_expand(ft_erase(result, k), &k, elements);//k n'est pas incremente, j'envoie qu'une copie.
		k++;
	}
	result[k] = '\0';
	return (result);
}
