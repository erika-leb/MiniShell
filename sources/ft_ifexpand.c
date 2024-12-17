

#include "../minishell.h"

// static char	*ft_geterrcode(void)
// {
// 	// int		err;
// 	// char	*str_err;

// 	// err = errno;
// 	// str_err = ft_itoa(err);
// 	// return (str_err);

// 	//ft_itoa présent dans Makefile, .h, mais inutilisé pour le moment
// 	//Il se peut que ft_itoa soit useless. Il faut juste récupérer le code
// 	//errno. On peut potentiellement le faire avec une variable globale.
// 	return("0");
// }

static char	*ft_getenvv(char *result, int *k, char *tmp)
{
    int i;

    i = 0;
 	while (result[*k + i] && (result[*k + i] == '_' || ft_isalnum(result[*k + i])))
 	{
 		tmp[i] = result[*k + i];
 		i++;
 	}
	//si i = 0 ici c'est peut etre qu'il y avait un ?, dans ce cas on regarde si c'est le cas et si oui
	//alors on incorpore 1 seul dans tmp avant de mettre \0. Ainsi si on ecrit $??? ca donne qqchose comme 0?? (comme dans bash)
	if (i == 0 && result[*k] == '?')
	{
		tmp[i] = '?';
		i++;
	}
    tmp[i] = '\0';
	if (!ft_strcmp(tmp, "?"))
		return (ft_itoa(0));
    return (getenv(tmp));
}

static void	ft_expand(char *result, int *k)
{
	char	tmp[20000];
	char	*envv;
	int		i;
	int		start;

	start = *k;
    envv = ft_getenvv(result, k, tmp);
    if (!envv)
        return (ft_erase_substr(result, k, tmp));
	//Si envv est full et contient des quotes alors on n'expand pas et on
	//remet le dollar. Je n'oublie pas d'ecraser tmp comme fait juste en dessous Apres ft_concat on pourra expand.
	// ... ... ...
    i = 0;
	while (tmp[i])
 	{
		//Dans result je copie/colle le debut de envv sur les caracteres de tmp
		if (envv[i] && result[*k] == tmp[i])
			result[*k] = envv[i];
		(*k)++;//On ecrase le nom de la var env
		i++;
 	}
	while (envv[i])
	{
		ft_insert(result, (*k), envv[i]);
		(*k)++;
		i++;
	}
	(*k)--;//permet de se retrouver sur le dernier caractere de la variable expand (le 'n' de hello/tuvabien)
	//Comme ca dans ft_isexpand on peut regarder le terme d'apres (qui peut etre un $)

	//ft_complexpand(result, start, k);//on modifie le tout et on incremente *k selon les besoins
}

static void	ft_delim(char *result, int *k, int sq, int dq)
{
	*k += 2;
	while (result[*k] == ' ')
		(*k)++;
	//si le 1er caractere du delim est une quote alors on avance jusqu'a revoir la meme quote suivie d'un espace
	//sinon on avance jusqu'a voir un espace
	ft_modifquote_(result, &sq, &dq, k);
	if (sq)
	{
		//sq est important car les var d'env inexistantes comme "  $HELLO" doivent apparaitre tel quel
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

static void	ft_ambig(char *result_k)
{
	//on cherche getenv et on regarde si ambiguous redirect.
	//si pas d'ambiguous on laisse ifexpand faire son travail.
	char		*envv;
	static char	tmp[20000];//il faut garder en copie le nom de la var d'env pour la reecrire dans le message d'erreur
	int			k;

	k = 0;
	if (result_k[k] != '$')
		return ;
	envv = NULL;
	if (result_k[k] == '$' && (result_k[k + 1] == '_'
		|| ft_isalnum(result_k[k + 1]) || result_k[k + 1] == '?'))
		envv = ft_getenvv(result_k + 1, &k, tmp);//getenvv remplie tmp
	if (!envv)
	{
		//il faudrait simplement remettre le dollar et le nom de la variable.
		//comme ca apres ft_split (et avant ft_concat qui transforme les '$a' en $a) on parcourt tous les token. La 1er var d'env
		//qui n'existe pas ou la 1ere redirection qui s'ouvre pas je renvoie le message d'erreur.
		//En effet le souci est que si j'ai une redirection qui s'ouvre pas avant un var env vide alors il faut afficher Permission denied
		//et pas ambiguous redirect.

		//On pourrait sinon laisser a Erika le soin d'expand pour voir si c'est ambiguous et ainsi afficher le bon message au bon endroit.
		//Le souci est que ft_concat transforme les '$a' en $a et donc je risque d'expand des var d'env qui sont cense rester en valeur
		//litterale.

		//Il faut ellaborer une strategie de gestion des erreurs bcp plus poussee avec une liste chainee par ex.

		//Dans bash il faut essayer d'expand la suite si jamais il y a plusieurs $. Et si aucune variable ne donne rien
		//alors on renvoie tout le nom de la var.
		// < $A$A$HOME retourne le HOME. Pour ifexpand normal (hors redirection) ca se gere tout seul on a pas d'erreur a indiquer.
		//		Tant que (result_k[++k] != '$' && ft_isalnum(result_k[k])) if ()


		printf("bash: $%s: ambiguous redirect\n", tmp);//il faut exit ?
	}
	
}

static void	ft_incrk(char *result, int *k)
{
	(*k)++;
	if (!ft_strncmp(result + *k, ">> ", 3))
		(*k)++;
	while (result[*k] == ' ')
		(*k)++;
}

char	*ft_ifexpand(char *result, int sq, int dq)
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
			if (!sq && !dq)
				ft_ambig(result + k);
		}
		//S'assurer qu'Erika n'a pas mis $ comme token, comme ca si je lui envoie $ c'est qu'elle doit le traiter comme sa valeur litterale.
		//ft_erase ecrase '$' en copiant/collant tous les elements a indice - 1, pour lancer ft_expand sur ce qui vient apres
		if (result[k] == '$' && !sq && (result[k + 1] == '_'
			|| ft_isalnum(result[k + 1]) || result[k + 1] == '?'))
			ft_expand(ft_erase(result, k), &k);//ft_erase(result, k);//k n'est pas incremente, j'envoie qu'une copie.
		k++;
	}
	result[k] = '\0';
	return (result);
}