#include "../minishell.h"
//atoi adapté ? Je crois dans les test y'a exit -----42.
//est ce que ca se comporte de la meme façon ? bash: exit: --42: numeric argument required
//mais par contre exit est quand meme execute et le retour d'erreur est command not found je crois
//Les code retour ne peuvent pas etre negatifs et sont convertis en positif :
//ex -42 donne 214. "exit" 42 est valide. "exit 42" n'est pas valide et correspond a une
//commande du meme nom.
//Il vaut mieux donc lancer exit apres ft_ft pour le traiter correctement


//J'ai remarqué que quand je fais un exit "  42" par ex, puis que je fais $? alors j'obtiens bien
// le code 42. Par contre si je refais $? j'obtiens le code 127. Why ?


//Il faut modifier ft_atoi pour que ca overflow quand le nb est negatif, et que ca
//renvoie l'erreur numeric argument required si j'entre un signe de trop ou un caractere
//autre.
int	ft_atoi(const char *str)
{
	int	i;
	int	neg;
	int	res;

	i = 0;
	neg = 1;
	res = 0;
	//Ce while est utile car exit "    42" est valide
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		neg = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	//else if c'est pas un numerique alors erreur numeric argument required
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	//avant de return on verifie que soit on est en bout de chaine, soit il y a
	//que des espaces avant le bout de chaine sinon c'est que dans la chaine y'a des trucs
	//invalides ex : 43253a
	return (res * neg);
}