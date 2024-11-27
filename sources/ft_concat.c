#include "../minishell.h"

//Erika pourra reutiliser cette fonction pour reperer les token litteraux
//if (ft_littoken(result_i)) alors ft_erase(result_i, 0); ft_erase(result_i, 1);
//les 2 premiers if ne seront jamais activés car je me suis chargé de modifier
//en amont les "<" ">" etc

static int	ft_onetoken(char *av2)
{
	int	i;

	i = 0;
	if ((av2[0] == '|' && !av2[1])
		|| (av2[0] == '<' && !av2[1])
		|| (av2[0] == '>' && !av2[1])
		|| (av2[0] == '<' && av2[1] == '<' && !av2[2])
		|| (av2[0] == '>' && av2[1] == '>' && !av2[2]))
		return (1);
	return (0);
}

//CA MARCHE PAS CAR <'' ne doivent pas etre concaténé mais considere comme 2 token differents.
//Pourtant pour <<'' et ''<< fonctionne bien.
// Il faut ameliorer le if (is_concat && ft_istok_(&result_i[0]))
// 							ft_insert(ft_insert(result_i, 0, '\''), ft_strlen(result_i), '\'');
//Le pb est que ma tokenization n'ajoute pas d'espaces quand j'ecris <'' et |''. Pour le reste ca marche.
//Comme ils sont en 1ere position, < et | n'enclenchent pas ft_addspace


//PAR AILLEURS >& est un token qui ecritce qu'il y a dans stderr vers le fichier specifierex : cucou >& hello
//va ecrire dans hello. Non en fait je pense que c'est good : juste dans le parsing si & n'est pas colle a >
//alors on met une erreur (voir l'erreur qu'affiche bash). Et je laisse a ma tokenization le soin de les separer puisque & est un fichier dans 
//Linux. PAS SUR, ILFAUT SE PEMCHER SUR LAQUESTION. Ce que medit chatGPT est trop chelou j'ai pas trop compris. Faire une conversation dediee.

//>& et <& servent a rediriger l'entree la sortie vers des fd deja ouverts. Exemple echo "Hello" > file.txt 2>&1. Ca ne doit pas etre géré
//Donc on laisse le code tel quel.

//Pour reduire nb de lignes de ft_concat agreger la fin du code (a partir du if) dans une fonction qui retourne result_i

char	*ft_concat(char *result_i, int k, int sq, int dq)
{
	int	to_erase;
	int	is_concat;

	while (result_i[++k] && !(result_i[k] == '\'' || result_i[k] == '\"'))
	if (!result_i[k + 1])
		return (result_i);
	to_erase = k;
	is_concat = 0;
	ft_modifquote_(result_i, &sq, &dq, &k);
	while (result_i[++k])
	{
		if (result_i[k] == result_i[to_erase])
		{
			is_concat = 1;
			ft_erase(result_i, to_erase);
			k--;//Comme on vient de supprimer un quote tout se decale vers la gauche
			ft_erase(result_i, k);
			k--;
			while (result_i[++k] && !(result_i[k] == '\'' || result_i[k] == '\"'))
			if (!result_i[k])
				return (result_i);
			to_erase = k;
		}
	}
	//On peut agrandir la chaine meme si c de la heap mallocé car vu qu'on a concat
	//on a libéré de la place qu'on reremplie de moitié apres
	if (is_concat && ft_onetoken(result_i))
		ft_insert(ft_insert(result_i, 0, '\''), ft_strlen(result_i), '\'');
	//printf("%zu\n", ft_strlen(result_i));//Verifie que les token nuls sont bien créés
	return (result_i);
}