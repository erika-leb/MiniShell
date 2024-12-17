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

static int	ft_istok_(char *av2)
{
	if (*av2 == '|' || *av2 == '<' || *av2 == '>'
		|| (*av2 == '<' && *(av2 + 1) == '<')
		|| (*av2 == '>' && *(av2 + 1) == '>'))
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

	//on peut pas commencer par un |
	if (!ft_strcmp(result[0], "|"))
		return(printf("bash: syntax error near unexpected token `|'\n"));
	i = 0;
	while (result[i])
	{
		//seul le | peut avoir < > << >> apres lui. Mais il peut pas avoir un autre |
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
