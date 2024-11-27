

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
	//Comme ca dans ft_isexpand on on peut regarder le terme d'apres (qui peut etre un $)
}

// test :'$HOME' "$HOME" "'$HOME'" '"$HOME"' pour comprendre ce qui se passe
char	*ft_ifexpand(char *result)
{
	int	k;
	int	sq;//ne pas expand
	int	dq;

	k = 0;
	sq = 0;
    dq = 0;
	while (result[k])
	{
		ft_modifquote(result, &sq, &dq, &k);
		//Si ce qui vient apres $ n'est pas un alphanumerique alors on le laisse tel quel
		//$? restera tel quel, "$" aussi, $ (tout seul) aussi (voir comportement bash).
		//ft_erase ecrase '$' en copiant/collant tous les elements a indice - 1
		//avant de lancer ft_expand (qui s'occupe de d'inserer la var env).
		//ft_erase(result, k);//k n'est pas incremente, j'envoie qu'une copie.

		//ATTENTION : "'"$HOME"'"         renvoie       '/home/aisidore': No such file or directory
		//Hors moi je retourne "'"/home/aisidore"'". Je pense que ft_concat va regler le pb
		//$_ renvoie ./minishell dans mon minishell tandis que le bash renvoie bash: '': command not found
		//normal ca retourne la derniere commande lancee.

		//S'assurer qu'Erika n'a pas mis $ comme token, comme ca si je lui envoie $ c'est que c'est une commande
		if (result[k] == '$' && !sq && (result[k + 1] == '_' || ft_isalnum(result[k + 1])))
			ft_expand(ft_erase(result, k), &k);	
		k++;
	}
	result[k] = '\0';
	return (result);
}