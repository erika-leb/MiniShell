/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ifexpand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aisidore <aisidore@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/12 19:19:25 by aisidore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static void	ft_spacequotes(char	*result_k, t_element *elements, t_gc *gc)
{
	char	tmp[20000];
	char	*envv;
	int		i;

	i = 1;
	// if (*result_k == '\"')
	// 	i++;//useless I guess
	if (*result_k != '$')
		return ;
	//Ici on est forcement sur un dollar. On expand ce qui vient apres le dollar et on l'analyse
	envv = ft_getenvv(result_k, &i, tmp, elements, gc);
	i = 0;
	// printf("tmp = %s taille = %zu\n", tmp, ft_strlen(tmp));
	while (envv && envv[i])
	{
		if (envv[i] == ' ' || envv[i] == '\'' || envv[i] == '\"')
		{
			ft_insert(result_k, 1, '\t');
			ft_insert(result_k, ft_strlen(tmp) + 2, '\t');
			return ;
		}
		i++;
	}
}

//adri42
//Si la taille de key < a la taille de name alors invalid read
static void	ft_expand(char *result, int *k, t_element *elements, t_gc *gc)
{
	char	tmp[20000];
	char	*envv;
	int		i;
	size_t		len;

	i = 0;
	len = 0;
    envv = ft_getenvv(result, k, tmp, elements, gc);//si y'a des quotes alors j'expand pas et envv prend la valeur de $\n\t ...
    if (!envv)
		return (ft_erase_substr(result, k, tmp));

	//On partait du principe que la taille de key etait superieur a name. Donc d'abord on ajoutait dans result par exemple 6 caracteres
	//si name est de taille 6. Puis la seconde boucle while insere les elements manquant en deplacant tous les caracters d'un cran
	//vers la droite. Maintenant on remplit ecrase les valeurs de tmp
	while (tmp[i] && envv[i])
 	{
		result[*k] = envv[i];
		(*k)++;
		i++;
 	}
	//Ensuite on insere les valeurs de envv restantes.
	while (envv[i])
	{
		ft_insert(result, (*k), envv[i]);
		(*k)++;
		i++;
	}
	//arrive ici on a ecrase les caracteres de tmp par ceux de env et on a inserer les caracteres restants de env.
	//Il faut maintenant supprimer l'excedent de caracteres de tmp qu'il pourrait rester
	if (ft_strlen(tmp) > ft_strlen(envv))
	{
		len = ft_strlen(tmp) - ft_strlen(envv);
		while (len)
		{
			ft_erase(result, *k);
			len--;
		}
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

//adri42ft_spacequotes(result);
char	*ft_ifexpand(char *result, int sq, int dq, t_element *elements, t_gc *gc)
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

			//Je ne peux pas utiliser ft_ambig car il ne s'applique pas si je suis en double quote et mine de rien je peux avoir des cas ou
			//le user fais export a="  "haha" 'ca va bien' " auquel cas j'a interet a aussi gerer le cas ou je suis dans des doubles quotes.
			if (!sq && !dq)
				ft_ambig(result + k, &k, elements, gc);
		}
		if (!sq && !dq)
			ft_spacequotes(result + k, elements, gc);//hedge : si je fais $zzz$HOME$zzz alors j'ai un espace qui se glisse (a la place de \t)
		//S'assurer qu'Erika n'a pas mis $ comme token, comme ca si je lui envoie $ c'est qu'elle doit le traiter comme sa valeur litterale.
		//ft_erase ecrase '$' en copiant/collant tous les elements a indice - 1, pour lancer ft_expand sur ce qui vient apres
		if (result[k] == '$' && !sq && (result[k + 1] == '_'
			|| ft_isalnum(result[k + 1]) || result[k + 1] == '?'))
			ft_expand(ft_erase(result, k), &k, elements, gc);//k n'est pas incremente, j'envoie qu'une copie.
		k++;
	}
	result[k] = '\0';
	return (result);
}
