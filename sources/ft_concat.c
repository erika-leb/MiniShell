#include "../minishell.h"

//Erika pourra reutiliser cette fonction pour reperer les token litteraux
static int	ft_onetoken(char *av2)
{
	if ((av2[0] == '|' && !av2[1])
		|| (av2[0] == '<' && !av2[1])
		|| (av2[0] == '>' && !av2[1])
		|| (av2[0] == '<' && av2[1] == '<' && !av2[2])
		|| (av2[0] == '>' && av2[1] == '>' && !av2[2]))
		return (1);
	return (0);
}

static char	*ft_retmerge(char *result_i, int is_concat)
{
	if (is_concat && ft_onetoken(result_i))
		ft_insert(ft_insert(result_i, 0, '\''), ft_strlen(result_i), '\'');// + 1
	//printf("%zu\n", ft_strlen(result_i));//Verifie que les tokens nuls sont bien crees
	return (result_i);
}

static void	ft_initconcat(int *to_erase, int k, int *is_concat)
{
	*to_erase = k;
	*is_concat = 0;
}

//SOUCI : si je créé une var d'env et que je lui attribue la valeur 'hello' alors ft_concat va modifier a tord
//cette variable. C'est pourquoi il vaut mieux expand les var d'env contenant des quotes apres ft_concat.
char	*ft_concat(char *result_i, int k, int sq, int dq)
{
	int	to_erase;
	int	is_concat;

	//Ici il faut une fonction qui analyse la chaine et regarde si y'a des $;a
	//Si oui c'est que la taille de result_i est inadapte, il va falloir recreer une chaine et l'arimer au tableau.
	//Ensuite si y'a des espaces et qu'on est pas devant une redirection il va falloir decouper  et creer plusieurs
	//result_i. Le souci est que ft_concat ne pourra pas retourner de char et dans le main ne peut pas etre egale a result_i.
	while (result_i[++k] && !(result_i[k] == '\'' || result_i[k] == '\"'))
	if (!result_i[k + 1])
		return (result_i);
	ft_initconcat(&to_erase, k, &is_concat);
	ft_modifquote_(result_i, &sq, &dq, &k);
	while (result_i[++k])
	{
		if (result_i[k] == result_i[to_erase])
		{
			is_concat = 1;
			ft_erase(result_i, to_erase);
			k--;
			ft_erase(result_i, k);
			k--;
			while (result_i[++k] && !(result_i[k] == '\''
					|| result_i[k] == '\"'))
			if (!result_i[k])
				return (result_i);
			to_erase = k;
		}
	}
	return (ft_retmerge(result_i, is_concat));
}