#include "../minishell.h"


//									PAS TRES PERTINENT
//									PARSING AVANT TOKENIZATION
//while (str[i]) si a un moment donné, je suis hors des quotes et ft_strncmp(str, "<<<", 3)
//alors Invalid token. Idem pour >>>, ||, &&, & tout seul [HORS DES QUOTES !]
// syntax error near unexpected token `|' ...

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
		return(printf("Unclosed brackets.\n"));
	return (0);
}

void	ft_ft(char *input)
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

//En debut d'execution je modifie la chaine : si je suis hors brackets, que je tombe sur un $ et qu ce qui suit c'est un quote
//alors je erase le dollar.




//Je m'inspire de ft_concat pour m'assurer que les macro quote sont fermées. Je refuse les quotes non fermées :
//	- 'hello " ' est valide car '' fermé
//	-'hello  '' invalide.			Unclosed brackets.


//									PARSING APRES TOKENIZATION

//Privilegier le parsing apres quand on est pret a l'emploi afin de savoir reellement qui est quoi
//TRES UTILE POUR LES CHOSES QUI CONCERNENT PAS LE PROJET : comme empecher le user d'entrer des quotes non fermés pour les delim de heredoc
//ou meme de facon generale. Les erreurs qui font qu'on execute meme pas la commande.

//PB Le pb du parsing c'est que seul l'erreur du dernier enfant doit apparaitre. Or, apres la tokenization,
//On ne sait pas encore delimiter les enfants. Ex : si j'ecris <<uu < uu | << seul l'erreur du 2eme enfant apparait.
//Je pense que le parsing est plus pertinent apres qu'Erika ait créé les noeuds. Genre si un noeud est labelisé REDIR
//et qu'il est suivi d'un autre noeud REDIR alors erreur.


// -----------------------------------------------------------------------------------------------------------

//ATTENTION si le delimiteur comporte des quote alors les variables d'environnement ne s'expand pas,
//sinon on utilise ft_ifexpand sur chaque newline de heredoc.


