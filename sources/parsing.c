#include "../minishell.h"


//									PAS TRES PERTINENT
//									PARSING AVANT TOKENIZATION
//while (str[i]) si a un moment donné, je suis hors des quotes et ft_strncmp(str, "<<<", 3)
//alors Invalid token. Idem pour >>>, ||, &&, & tout seul [HORS DES QUOTES !]
// syntax error near unexpected token `|' ...
//Refuser plus d'un $ sauf s'il est dans des single quotes, car ca donne le PID du processus en cours ???

//Je m'inspire de ft_concat pour m'assurer que les macro quote sont fermées. Je refuse les quotes non fermées :
//	- 'hello " ' est valide car '' fermé
//	-'hello  '' invalide.			Unclosed brackets.


//									PARSING APRES TOKENIZATION

//Privilegier le parsing apres quand ton est pret a l'emploi afin de savoir reellement qui est quoi

//PB Le pb du parsing c'est que seul l'erreur du dernier enfant doit apparaitre. Or, apres la tokenization,
//On ne sait pas encore delimiter les enfants. Ex : si j'ecris <<uu < uu | << seul l'erreur du 2eme enfant apparait.


// -----------------------------------------------------------------------------------------------------------

//ATTENTION dans heredoc si le user mets en délimiteur '<'ls alors le delimiteur sera <ls
// Il faut donc utiliser ft_concat dans le heredoc> Ex <<'xx <'ls'ca va' donne en delimiteur : xx <lsca va.
//Autre exemple : 'hello' donne hello en delimiteur.
//ATTENTION si le delimiteur comporte des quote alors les variables d'environnement ne s'expand pas,
//sinon on utilise ft_ifexpand sur chaque newline de heredoc.


