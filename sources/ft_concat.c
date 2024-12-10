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

char	*ft_concat(char *result_i, int k, int sq, int dq)
{
	int	to_erase;
	int	is_concat;

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