#include "../minishell.h"

int ft_checkq(char *input)
{
    int sq;
    int dq;
    char *ptr;

	sq = 0;
	dq = 0;
	ptr = input;
    while (*ptr)
	{
		if (!sq && !dq && *ptr == '$' && *(ptr + 1) == '$')
			return(printf("bash: syntax error near unexpected token `$'\n"));
        if (*ptr == '\'' && !dq)
			sq = !sq;
        else if (*ptr == '\"' && !sq)
			dq = !dq;
        ptr++;
    }
	if (sq || dq)
		return(printf("bash: unclosed quotes\n"));
	return (0);
}

int	ft_istok_(char *av2)
{
	if (!ft_strcmp(av2, "|") || !ft_strcmp(av2, "<") || !ft_strcmp(av2, ">")
		|| !ft_strcmp(av2, "<<") || !ft_strcmp(av2, ">>"))
		return (1);
	return (0);
}

//Si Erika recoit un result NULL c'est qu'une erreur fatale a eu lieu,
//il faut juste renvoyer la ligne.

//Si j'ai ambiguous redirect alors l'erreur newline s'affiche aussi.
//si je veux eviter ca je peux remplir un buffer d'erreur
int	ft_unexptoken(char **result)
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

//Si le user ecrit $'HAHA' (hors quotes) alors on retient 'HAHA'
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

void	ft_ft(char *line, char **result, int i, int go)
{
	//Erika doit remplacer result par elements.
	if (!ft_checkq(line))
		go = 1;
	result = ft_split(ft_tokenize(line), 0, 0);
	while (go && result && result[++i])
		result[i] = ft_concat(result[i], -1, 0, 0);
	if (result && ft_unexptoken(result))
		go = 0;
	i = -1;
	while (go && result && result[++i])
		printf("token %d :%s\n", i, result[i]);
	if (go && result)
		ft_freesplit(result, i);
}


// if (!ft_checkq(line))
// 	result = ft_split(ft_tokenize(line), 0, 0);
// else
// 	result = NULL;//Si des quotes sont pas fermes on malloc rien et on retourne result == NULL
// while (result && result[i])
// {
// 	result[i] = ft_concat(result[i], -1, 0, 0);
// 	i++;
// }
// if (ft_unexptoken(result))
// {//si des token sont cote a cote a tord alors on free et on retourne result == NULL
// 	ft_freesplit(result, i);
// 	result = NULL;
// }
// i = 0;
// while (result && result[i])
// {
// 	printf("token %d :%s\n", i, result[i]);
// 	i++;
// }