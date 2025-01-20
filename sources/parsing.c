#include "../minishell.h"

static int ft_checkq(char *input)
{
	int		sq;
	int		dq;
	char	*ptr;

	sq = 0;
	dq = 0;
	ptr = input;
	while (*ptr)
	{
		if (!sq && !dq && *ptr == '$' && *(ptr + 1) == '$')
			return(printf("bash: syntax error near unexpected token `$'\n"));//write mais useless
		if (*ptr == '\'' && !dq)
			sq = !sq;
		else if (*ptr == '\"' && !sq)
			dq = !dq;
		ptr++;
	}
	if (sq || dq)
		return(printf("bash: unclosed quotes\n"));//write mais useless
	return (0);
}

int	ft_istok_(char *av2)
{//static ?
	if (!ft_strcmp(av2, "|") || !ft_strcmp(av2, "<") || !ft_strcmp(av2, ">")
		|| !ft_strcmp(av2, "<<") || !ft_strcmp(av2, ">>"))
		return (1);
	return (0);
}

//Si Erika recoit un result NULL c'est qu'une erreur fatale a eu lieu,
//il faut juste renvoyer la ligne : je le fais deja je pense.
static int	ft_unexptoken(char **result)
{
	int	i;

	//\0 est un unexpected token
	if (!(*result))
		return (1);//Sinon ecrire juste $a (qui ne correspond a rien) provoquait un segfault a cause de ft_strcmp
	if (!ft_strcmp(result[0], "|"))
		return(printf("bash: syntax error near unexpected token `|'\n"));
	i = 0;
	while (result[i])
	{
		if (!ft_strcmp(result[i], "|") && result[i + 1] && !ft_strcmp(result[i + 1], "|"))
			return(printf("bash: syntax error near unexpected token `|'\n"));
		if (ft_istok_(result[i]) && ft_strcmp(result[i], "|")
			&& result[i + 1] && ft_istok_(result[i + 1]))
			return(printf("bash: syntax error near unexpected token `%s'\n", result[i + 1]));
		if (ft_istok_(result[i]) && !result[i + 1])
			return(printf("bash: syntax error near unexpected token `newline'\n"));
		//contrairement a bash on refuse de prendre les infos en dquote si le user met un | en fin de commande.
		i++;
	}
	// if (*error_)
	// 	return(printf("%s\n", error_));
	return (0);
}

void	ft_deldollar(char *input)
{
	int	sq;
	int	dq;
	int	i;

	sq = 0;
	dq = 0;
	i = 0;
	while (input[i])
	{
		ft_modifquote_(input, &sq, &dq, &i);
		if (!sq && !dq && input[i] == '$'
			&& (input[i + 1] == '\'' || input[i + 1] == '\"'))
			ft_erase(input, i);
		i++;
	}
}

//Rechecker la logique de ce truc.
//Remplacer result par elements.
void	ft_ft(t_element *elements, t_gc *gc)
{
	int	i;
	int	go;

	i = -1;
	go = 0;
	if (!ft_checkq(elements->line))
		go = 1;
	//SI ON VEUT UILISER MALLOC PLUTOT QUE STATIC CHAR ALORS ft_tokenize DOIT CONTENIR gc
	elements->arr = ft_split(ft_tokenize(elements->line, gc), 0, 0, gc);
	while (go && elements->arr && elements->arr[++i])
		elements->arr[i] = ft_concat(elements->arr[i], -1, 0, 0);
	if (elements->arr && ft_unexptoken(elements->arr))
		go = 0;
	i = -1;
	while (go && elements->arr && elements->arr[++i])
		printf("token %d :%s\n", i, elements->arr[i]);//juste pour checker, useless sinon
	// if (go && elements->arr)
	// 	(gc_cleanup(gc), free_std(), exit(2));//2 est arbitraire, je suis pas sur. ft_freesplit(result, i);
}
