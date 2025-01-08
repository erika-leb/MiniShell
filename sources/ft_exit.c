#include "../minishell.h"

//Est ce que faire un exit suffit ? Les autres enfants ont ils egalement acces a ce code d'erreur ?
//Il faudrait creer une variable globale et modifier sa valeur.
//Demander aux gens comment ils font pour acceder au dernier code d'erreur (var globale ? Autre ?)

//Si y'a plus d'1 arguments alors exit ne s'execute pas !!! Ce sera a retranscrire
//quand j'integrerai exit dans le code d'Erika

static void ft_exitfail(const char *str)
{
    //un write (le dup2 souhaite, "bash: exit: %s: numeric argument required\n", ft_strlen);
    //sera plus approprie. Il faudra juste incorporer le %s avant de mettre le tout dans write.
	printf("bash: exit: %s: numeric argument required\n", str);
	exit(2);
}

static void ft_initexit(const char *str, int *i, int *neg, long *res)
{
	*i = 0;
    *neg = 1;
    *res = 0;
	//Pour gerer les exit "   42" :
	while ((str[*i] >= '\t' && str[*i] <= '\r') || str[*i] == ' ')
        (*i)++;
}

static void ft_fillexit(const char *str, int *i, int neg, long *res)
{
    int	curr_digit;
    // Convertir la chaîne en long tout en vérifiant les débordements (voir GPT pour explications)
    //Si le nb ne peut pas etre contenu dans un long alors ca renvoie l'erreur numeric argument recquired.
    while (str[*i] >= '0' && str[*i] <= '9')
    {
        curr_digit = str[*i] - '0';

        // Vérifier les débordements avant d'ajouter le chiffre
        if ((neg == 1 && *res > (LONG_MAX - curr_digit) / 10) ||
            (neg == -1 && *res > (LONG_MIN + curr_digit) / -10))
            ft_exitfail(str);
        *res = (*res) * 10 + curr_digit;
        (*i)++;
    }
	//Pour gerer les cas exit "42   "
    while ((str[*i] >= '\t' && str[*i] <= '\r') || str[*i] == ' ')
        (*i)++;
}

int ft_exit(const char *str)
{
    int i;
    int neg;
    long res;

    //si plus d'1 argument alors bash: exit: too many arguments
    //et on modifie le code d'erreur pour qu'il soit egal a 2.
    //C'est pour ca que mon ft_exit prendra en argument non pas const char *str
    //mais plutot un truc du style une liste chainee a analyser.

	ft_initexit(str, &i, &neg, &res);
    if (str[i] == '-')
    {
        neg = -1;
        i++;
    }
    else if (str[i] == '+')
        i++;
	//Pour eviter que le user mette des trucs du style "  + " ou +a :
    if (!(str[i] >= '0' && str[i] <= '9'))
        ft_exitfail(str);
    ft_fillexit(str, &i, neg, &res);
	//avant de return on verifie que soit on est en bout de chaine, soit il y a
	//que des espaces avant le bout de chaine sinon c'est que dans la chaine y'a des trucs
	//invalides ex : 43253a.
    if (str[i])
        ft_exitfail(str);
    if (neg == -1)
        res = -res;
    exit((unsigned int)res % 256);
}

//gcc -o ft_exit sources/ft_exit.c
//./ft_exit 42
//echo $?
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <exit_code>\n", argv[0]);
        return 1;
    }
    printf("Calling ft_exit with argument: %s\n", argv[1]);
    ft_exit(argv[1]);
	printf("Haha");
    return 0;
}