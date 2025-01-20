
#include "../minishell.h"

//insere un caractere a l'endroit k, augmente la taille de la chaine de + 1
char *ft_insert(char *result, int k, char c)
{
	int len;
	int i;

	len = ft_strlen(result);
	i = len;
	while (i >= k)
	{
		result[i + 1] = result[i];
		i--;
	}
	result[k] = c;
	return (result);
}

//Supprime un caractere au niveau de tmp_k, reduit la taille de la chaine de -1.
//Le vrai indice (k) n'est pas modifie et reste au niveau du caractere supprime
char *ft_erase(char *result, int tmp_k)
{
	while (result[tmp_k])
	{
		result[tmp_k] = result[tmp_k + 1];
		tmp_k++;
	}
	return (result);
}

void ft_erase_substr(char *result, int *k, char *tmp)
{
    int i;

    //Prend en parametre la string a supprimer (ou de moins un string de meme taille), l'indice ou se trouve
    //la string, et la chaine dans laquelle on veut supprimer
    i = 0;
    //Comme la variable env n'existe pas, on la retire de la string :
	//On decale vers la gauche autant de fois que la variable env a de caractere (HOME= 4 fois)
	while(tmp[i++])
		ft_erase(result, *k);//*k n'est pas incremente, j'envoie qu'une copie
	//L'indice k se situe a l'endroit du caractere supprime, par ex la ou y'avait le $,
	//donc je recule d'1 pour pouvoir bien analyser ce qui a remplace le $.
	(*k)--;
	return ;//Adapte si y'a des double quote ? autour ? Voir bloc note
}

//Identique a celui dans ft_split.c. Mais je l'ai refait car lautre prend size_t *i en param
//et javais la flemme de changer le prototype de toutes les fcts.
//Mettre un pointeur a i est useless car on modifie pas sa valeur
void	ft_modifquote_(char const *str, int *sq, int *dq, int *i)
{
    //Mettre une adresse pour i est useless car on change pas sa val
	if (str[*i] == '\'' && !(*dq))
		*sq = !(*sq);
	if (str[*i] == '\"' && !(*sq))
		*dq = !(*dq);
}
