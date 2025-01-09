#include "../minishell.h"

// gerer echo -nnnnnnnnnnn bonjour (affiche bonjourbash-5.1$)
//echo -n -n -n ... -l bonjour (affiche -l bonjourbash-5.1$)
//echo -nnnnnnnnl (affiche -nnnnnnl bonjour [\n]bash-5.1$)
//echo bonjour > /dev/full (affiche un message d'erreur car interdiction d'ecrire dans ce fichier)
//if (errno == ENOSPC) write message d'erreur

//echo recupere (dans cet ordre) : 1) 1 ou plsr fois l'argument -n ("-nnn" "-n -n -n")
//                                 2) un tableau de chaine de caracteres en arguments ("bonjour" "ca va" bien "-nl")

//si j'ecris juste echo, ca renvoie un retour a la ligne

void    ft_echo(const char **opt_arg)
{
    
}