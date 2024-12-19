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

static void	ft_ibslash(char *result_k, char *name, int *k)
{
	int	m;

	m = 0;
	(*k)++;//car on ajoute un \n
	ft_insert(result_k, 0, '\n');
	//on fait en sorte que if_expand zappe tous les caracteres de name (dans result).
	while (name[m])
	{
		m++;
		(*k)++;
	}
	//printf("bash: %s: ambiguous redirect\n", name);
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
	m = -1;
	while (result_k[++m] && result_k[m] != ' ')
		name[m] = result_k[m];
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
		ft_ibslash(result_k, name, k);
	// && !name[m] : Si j'ecris < $u"HOME" alors grace au !name[m]  et au if (envv || !ft_moredoll(name + 1 + m)) j'entre pas dans le if.
	//En effet dans "HOME" il n'y a pas de dollars hors quote et donc le break du dessus s'enclenche.
}