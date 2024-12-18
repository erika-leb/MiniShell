#include "../minishell.h"

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

void	ft_ambig(char *result_k, int *k)
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
	if (!envv && !name[m])
	{// && !name[m] : Si j'ecris < $u"HOME" alors grace au !name[m]  et au if (envv || !ft_moredoll(name + 1 + m)) j'entre pas le if.
		//En effet dans "HOME" il n'y a pas de dollars hors quote et donc le break du dessus s'enclenche.

		//Le user ne peut pas ouvrir des quotes sans les fermer. Il ne peut donc pas ecrire $'a. La seule maniere d'obtenir ce resultat
		//est d'ecrire '$'"'"a OU "$""'"a (ce qui est tres improbable). Ainsi si Erika recoit $'a elle saura que la var d'env a est NULL.
		//Pour identifier ce type de var d'env elle doit la parcourir et ne tomber que sur des caracteres alphanumeriques, des _ ou des $.

		//Quid si le user entre '$$a' ? Il faut intercepter ce cas dans ft_concat.
		//Si Erika tombe sur $$ et un alphanum c'est qu'il s'agit d'une variable expand qui n'existe pas, elle doit donc
		//tej 1 $ et recuperer tout ce qui vient apres pour faire le message d'erreur.
		//Si elle tombe sur un token '$$[alphanum]' (avec ft_strncmp) c'est qu'elle doit traiter la chose comme une valeur litterale
		// (*k)++;//car on ajoute un $
		// ft_insert(result_k, 0, '$');
		// m = 0;
		// //on fait en sorte que if_expand zappe tous les caracteres de name (dans result).
		// while (name[m])
		// {
		// 	m++;
		// 	(*k)++;
		// }
		// printf("bash: $%s: ambiguous redirect\n", name + 1);//il faut exit ?


		//On peut faire sinon avec un \n !!! On reprend le code du dessous n faisant (*k)++ etc...
		
		//Donc on peut autoriser les $$ ? Je peux aussi l'utiliser pour les var env avec espaces et quotes
		ft_insert(ft_insert(ft_insert(result_k, 1, '\"'), 1, '\''), 1, '\"');
		(*k) += 3;//car on ajoute un $
		printf("%s\n", result_k);
		m = 0;
		//on fait en sorte que if_expand zappe tous les caracteres de name (dans result).
		while (name[m])
		{
			m++;
			(*k)++;
		}
		printf("bash: $%s: ambiguous redirect\n", name + 1);//il faut exit ?
	}
	
}