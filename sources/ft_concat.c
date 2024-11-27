#include "../minishell.h"

//Erika pourra reutiliser cette fonction pour reperer les token litteraux
//if (ft_littoken(result_i)) alors ft_erase(result_i, 0); ft_erase(result_i, 1);
//les 2 premiers if ne seront jamais activés car je me suis chargé de modifier
//en amont les "<" ">" etc
static int		ft_littoken(char *result_i)
{
	if (!ft_strcmp(result_i, "\"<\"")
		|| !ft_strcmp(result_i, "\">\"")
		|| !ft_strcmp(result_i, "\"|\""))
	{
		result_i[0] = '\'';
		result_i[2] = '\'';
	}
	if (!ft_strcmp(result_i, "\"<<\"")
		|| !ft_strcmp(result_i, "\">>\""))
	{
		result_i[0] = '\'';
		result_i[3] = '\'';
	}
	if (!ft_strcmp(result_i, "\'<\'")
		|| !ft_strcmp(result_i, "\'>\'")
		|| !ft_strcmp(result_i, "\'<<\'")
		|| !ft_strcmp(result_i, "\'>>\'")
		|| !ft_strcmp(result_i, "\'|\'"))
		return (1);	
	return (0);
}

char	*ft_concat(char *result_i, int k, int sq, int dq)
{
	int	to_erase;

	if (ft_littoken(result_i))
		return (result_i);
	while (result_i[++k] && !(result_i[k] == '\'' || result_i[k] == '\"'))
	if (!result_i[k])
		return (result_i);
	to_erase = k;
	ft_modifquote(result_i, &sq, &dq, &k);
	//k++;
	while (result_i[++k])
	{// && !ft_littoken(result_i)
		if (result_i[k] == result_i[to_erase])
		{
			//Que pasa si y'a un seul apostrophe ?
			//Ca passe puisqu'elle n'est pas du meme type
			ft_erase(result_i, to_erase);
			k--;//Comme on vient de supprimer un quote tout ce decale vers la gauche
			ft_erase(result_i, k);
			k--;
			while (result_i[++k] && !(result_i[k] == '\'' || result_i[k] == '\"'))
			if (!result_i[k])
				return (result_i);
			// if ()
			// {
			//	gestion des cas <'' ''< etc
			// 	//if(ft_istok(result_i) && ft_isquote(result_i[k]) && ft_isquote(result_i[k + 1]))
			// }
			to_erase = k;
		}
		//printf("%s\n",result_i);
		//k++;
	}
	return (result_i);
}