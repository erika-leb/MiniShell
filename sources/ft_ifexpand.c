

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

static int	ft_moredoll(char *str)
{
	int	i;
	int	sq;
	int	dq;

	//< $u$j : apres j il n'y a plus rien. Or cela provoquait un comportement indesirable
	//car ma fonction ft_moredoll indiquait (a raison) qu'il n'y avait plus de $ apres j,
	//cela entrainait un break et m'empechait de rentrer dans le if (!envv && !name[m]).
	if (!str[1])
		return (1);
	i = 0;
	sq = 0;
	dq = 0;
	while (str[i])
	{
		ft_modifquote_(str, &sq, &dq, &i);
		if (str[i] == '$' && !sq && !dq)
			return (1);
		i++;
	}
	return (0);
}

static void	ft_ambig(char *result_k, int *k)
{
	//on cherche getenv et on regarde si ambiguous redirect.
	//si pas d'ambiguous on laisse ifexpand faire son travail.
	char	*envv;
	char	tmp[20000];
	char	name[20000];
	int		m;

	if (*result_k != '$')
		return ;
	envv = NULL;
	m = 0;
	while (result_k[m] && result_k[m] != ' ')
	{
		name[m] = result_k[m];
		m++;
	}
	name[m] = '\0';
	m = 0;
	while (name[m])
	{
		if (name[m] == '$' && (*(name + m + 1) == '_'
			|| ft_isalnum(*(name + m + 1)) || *(name + m + 1) == '?'))
			envv = ft_getenvv(name + 1, &m, tmp);
		//Si je vois qu'apres (en name + 1 + m) il n'y a plus de dollars (hors quotes) alors je peux m'arreter
		if (envv || !ft_moredoll(name + 1 + m))
			break ;
		m++;
	}
	printf("%s\n", name + m);
	if (!envv && !name[m])
	{// && !name[m] : Si j'ecris < $u"HOME" alors grace au !name[m]  et au if (envv || !ft_moredoll(name + 1 + m)) j'entre pas le if.
		//En effet dans "HOME" il n'y a pas de dollars hors quote et donc le break du dessus s'enclenche.

		//Cette methode permet a Erika d'identifier les var d'env vides, ou elles se situent et donc faire peter l'enfant.
		//Ca lui permet aussi de savoir la priorite d'apparition du message d'erreur.

		//Quid si le user entre '$$a' ? Il faut intercepter ce cas dans ft_concat.
		//Si Erika tombe sur $$ et un alphanum c'est qu'il s'agit d'une variable expand qui n'existe pas, elle doit donc
		//tej 1 $ et recuperer tout ce qui vient apres pour faire le message d'erreur.
		//Si elle tombe sur un token '$$[alphanum]' (avec ft_strncmp) c'est qu'elle doit traiter la chose comme une valeur litterale

		(*k)++;//car on ajoute un $
		ft_insert(result_k, 0, '$');
		m = 0;
		while (name[m])
		{
			m++;
			(*k)++;
		}

		printf("bash: $%s: ambiguous redirect\n", name + 1);//il faut exit ?
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