

#include "../minishell.h"

static char	*ft_getenvv(char *result, int *k, char *tmp)
{
    int i;

    i = 0;
 	while (result[*k + i] && (result[*k + i] == '_' || ft_isalnum(result[*k + i])))
 	{
 		tmp[i] = result[*k + i];
 		i++;
 	}
    tmp[i] = '\0';
    return (getenv(tmp));
}

static void	ft_expand(char *result, int *k)
{
	char	tmp[20000];
	char	*envv;
	int		i;

    envv = ft_getenvv(result, k, tmp);
    if (!envv)
        return (ft_erase_substr(result, k, tmp));
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
	// if (!result[*k])
	// 	return ;
	//si le 1er caractere du delim est une quote alors on avance jusqu'a revoir la meme quote suivie d'un espace
	//sinon on avance jusqu'a voir un espace
	ft_modifquote_(result, &sq, &dq, k);
	if (sq)
	{
		(*k)++;
		while (result[*k] && !(result[*k] == '\'' && result[*k + 1] == ' '))
			(*k)++;
	}
	else if (dq)
	{
		(*k)++;
		while (result[*k] && !(result[*k] == '\"' && result[*k + 1] == ' '))
			(*k)++;
	}
	else
	{
		while (result[*k] && result[*k] != ' ')
			(*k)++;
	}
}

char	*ft_ifexpand(char *result, int sq, int dq)
{
	int	k;

	k = 0;
	while (result[k])
	{
		ft_modifquote_(result, &sq, &dq, &k);
		//Si je suis hors des quotes (sq et dq) et que ft_strncmp(result + k, 3, "<< ") c'est que j'ai repéré un heredoc
		//qui a ete prealablement separé par ft_tokenize. J'avance de +2 et je met exp = 0. Je me situe donc forcement
		//sur un espace normalement. Il ne faut pas expand ce qui vient apres.
		//et donc je modifie aussi le if pour ne pas qu'il s'enclenche.
		if (!sq && !dq && !ft_strncmp(result + k, "<< ", 3))
		{
			k += 2;
			while (result[k] == ' ')
				k++;
			//printf ("%s\n", result + k);
			//On arrive au niveau du delim qui peut etre composé ou non d'espaces
			//Il faut trouver un moyen de faire avancer k jusqu'a la fin du delimiteur (on pourra alors se passer de exp)
			ft_delim(result, &k, 0, 0);//On est forcement hors quote donc sq = 0 et dq = 0 en param
		}
		//Dès que j'ai fini de passer ce qui vient apres <<,
		//je dois pouvoir de nouver expand (trouver une solution pr regler ce pb). Selon moi, apres le k++; il faut reperer
		//le 1er espace en dehors de quotes qui marque la fin de la non expansion due à <<.

		//Si ce qui vient apres $ n'est pas un alphanumerique alors on le laisse tel quel
		//$? restera tel quel, "$" aussi, $ (tout seul) aussi (voir comportement bash).
		//Il faudra donc qu'Erika ajoute le token $? qui engendrera une fonction built_in dédiée à gérer
		//cette commande bash (comme demandé dans l'énoncé).

		//$_ renvoie ./minishell dans mon minishell tandis que le bash renvoie bash: '': command not found
		//normal ca retourne la derniere commande lancee.

		//S'assurer qu'Erika n'a pas mis $ comme token, comme ca si je lui envoie $ c'est que c'est une commande
		//ft_erase ecrase '$' en copiant/collant tous les elements a indice - 1, pour lancer ft_expand sur ce qui vient apres
		if (result[k] == '$' && !sq
			&& (result[k + 1] == '_' || ft_isalnum(result[k + 1])))
			ft_expand(ft_erase(result, k), &k);//ft_erase(result, k);//k n'est pas incremente, j'envoie qu'une copie.
		k++;
	}
	result[k] = '\0';
	return (result);
}