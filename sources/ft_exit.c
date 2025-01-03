#include "../minishell.h"
//atoi adapté ? Je crois dans les test y'a exit -----42.
//est ce que ca se comporte de la meme façon ?

//On peut meme lancer ft_exit avant ft_ft non ? Genre quand on fait le ft_strcmp ?
//Il faudra aussi que ca fonctionne si le exit est dans un enfant. Mais au moins si
//On le lance la ca evite de lancer tous le processus enfant. Il faudrait alors que
//ft_strcmp devienne ft_strncmp puisqu'exit prend des arguments ?
//En vrai c un peu useless car si le user s'amuse a mettre des espaces avant exit etc
int	ft_atoi(const char *str)
{
	int	i;
	int	neg;
	int	res;

	i = 0;
	neg = 1;
	res = 0;
	while ((str[i] >= '\t' && str[i] <= '\r') || str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		neg = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * neg);
}