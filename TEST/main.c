#include <stdio.h>
#include <stdlib.h>  // Pour calloc
#include <string.h>  // Pour strlen

#define TAILLE_MAX 70000  // Taille de la chaîne (70 000)

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	size_t			i;
	unsigned char	*s1;
	unsigned char	*s2;

	i = 0;
	s1 = (unsigned char *)str1;
	s2 = (unsigned char *)str2;
	while ((str1[i] || str2[i]) && i < n)
	{
		if (str1[i] != str2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

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

void	ft_modifquote_(char const *str, int *sq, int *dq, int *i)
{
    //Mettre une adresse pour i est useless car on change pas sa val
	if (str[*i] == '\'' && !(*dq))
		*sq = !(*sq);
	if (str[*i] == '\"' && !(*sq))
		*dq = !(*dq);
}

static int  ft_isinquote(char *result, int start)
{
    int i;
    int sq;
    int dq;

    i = 0;
    sq = 0;
    dq = 0;
    while (i < start)
    {
        ft_modifquote_(result, &sq, &dq, &i);
        i++;
    }
    if (sq || dq)
        return (1);
    return (0); 
}

static int  ft_isfull_withq(char *result, int *k, char *envv, char *tmp)
{
    //Cette fonction sert a reinserer $nom si la var d'env est pleine
    //mais contient des quotes
    int i;

    i = 0;
    while (envv[i])
    {
        if (envv[i] == ' ')
            return (0);//Il y a un espace, inutile d'aller plus loin
        i++;
    }
    //Pas d'espaces on check maintenant les quotes
    i = 0;
    while (envv[i])
    {
        if (envv[i] == '\'' || envv[i] == '\"')
        {
            while (tmp[j])
            {
                ft_insert(result, *k, tmp[*k])
                (*k)++;
            }
            return (1);
        }
        i++;
    }
    return (0);//Il n'y a pas non plus de quotes
}

static void	ft_complexpand(char *result, int start, int *k)
{

    //1) si la var d'env se situe dans des quotes alors
    //y'a rien a faire. Pour savoir on part de 0 et
    //on va jusqu'a start et si sq ou dq est vrai alors on return ;
    if (ft_isinquote(result, start))
        return ;
    if (ft_isfullnoq(result, start, k))
        return ;

    (*k)++;
    ft_insert(ft_insert(result, *k, '\"'), start, '\"');
    (*k)++;//k se situe au niveau de la derniere quote

    //2) si on est hors quote et que la chaine est full, alors
    //on fait rien. S'il s'agit d'un token alors on met des quotes simples.

    //Enfin si elle contient des quote
    //idealement faudrait pas l'expand et laisser son nom tel quel.
    //Apres ft_concat on pourrait alors envisager de l'expand au sein du token


    //3) si la var d'env n'est pas full (contient au moins 1 space)
    //et est sans quote alors on ajoute des dq en debut
    //et fin de chaine :
    //3bis) si en + de ca la var d'env ne se situe pas avant une redirection
    //alors on ajoute des " " entre chaque mot.

    //4) si la var d'env n'est pas full et contient des sq et/ou dq
    //alors on fait comme 3 sauf qu'on s'assure que les quotes delimiteurs
    //sont differentes de celles presents dans la chaine.


}

int main(int argc, char *argv[]) {
    // Vérification du nombre d'arguments passés
    if (argc != 3) {
        printf("Usage: %s <chaine>\n", argv[0]);
        return 1;
    }

    // Récupération de la chaîne entrée par l'utilisateur (av[1])
    char *input_str = argv[1];
    size_t input_length = strlen(input_str); // Longueur de la chaîne entrée

    // Allocation de mémoire pour 70 000 caractères
    char *chaine = (char *)calloc(TAILLE_MAX + 1, sizeof(char)); // +1 pour le caractère nul '\0'

    // Copie de la chaîne entrée par l'utilisateur dans la mémoire allouée
    // Si la chaîne est plus grande que 70 000 caractères, elle est tronquée.
    strncpy(chaine, input_str, TAILLE_MAX);

    // Affichage de la chaîne modifiée
    int k = strlen(chaine) - 1;
    ft_complexpand(chaine, atoi(argv[2]), &k);
    printf("Chaîne entrée par l'utilisateur : %s  %ld    chaine[k] : %c\n", chaine, strlen(chaine), chaine[k]);

    // Libération de la mémoire allouée
    free(chaine);

    return 0;
}

