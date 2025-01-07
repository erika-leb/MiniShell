

#include "../minishell.h"

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



static void	ft_incrk(char *result, int *k)
{
	(*k)++;
	if (!ft_strncmp(result + *k, "> ", 2))
		(*k)++;
	while (result[*k] == ' ')
		(*k)++;
}

//Dans ifexpand on pourrait inserer a differents endroits de la fonction (ou des sous fonctions)
//des checkers pour savoir si la var contient des espaces et/ou des quotes et ainsi expand ou non.
//Par ex si t'as var contient des espaces (pas sq ni dq) et qu'elle n'est pas apres une redir alors
//on peut expand sans se poser de questions.
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
				ft_ambig(result + k, &k);
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