#include "../minishell.h"

//									PARSING AVANT TOKENIZATION
//while (str[i]) si a un moment donné, je suis hors des quotes et ft_strncmp(str, "<<<", 3)
//alors Invalid token. Idem pour >>>, ||, &&, & tout seul [HORS DES QUOTES !]
// syntax error near unexpected token `|' ...
//Refuser plus d'un $ sauf s'il est dans des single quotes, car ca donne le PID du processus en cours ???

//Je m'inspire de ft_concat pour m'assurer que les macro quote sont fermées. Je refuse les quotes non fermées :
//	- 'hello " ' est valide car '' fermé
//	-'hello  '' invalide.			Unclosed brackets.


//									PARSING APRES TOKENIZATION


//Pb : si j'ecris ';' alors apres ft_concat le token final sera : ;. Hors il faut qu'on puisse
//distinguer ';' de ; (sur bash ca donne pas la meme erreur). Idem pour '\' et \.
//SOLUTION : intégrer une partie du parsing dans ft_littoken car à cette étape les token sont quasi pret.
//Ainsi si ft_strcmp(result_i, "\\") == 0 alors on refuse et on free split ! Idem pour ;
//Y a t il d'autres cas comme ca a gerer ? Car dans la consigne car la consigne n'est pas claire.

//JE VOIS MARQUE >& NUL PART. JE LAVAIS VU OU ?? EST CE QUE DU COUP C'EST AUSSI UN TOKEN INTERDIT COMME ; et \ ??
//Meme pb pour '>&' et >& qui doit pouvoir etre traité. Il faut ajouter ce token dans ft_istok etc et le traiter
//comme <, > et | (si y'a marqué ">&" alors c'est l'expression litterale '>&')

// -----------------------------------------------------------------------------------------------------------

//ATTENTION dans heredoc si le user mets en délimiteur '<'ls alors le delimiteur sera <ls
// Il faut donc utiliser ft_concat dans le heredoc> Ex <<'xx <'ls'ca va' donne en delimiteur : xx <lsca va.
//Autre exemple : 'hello' donne hello en delimiteur.
//ATTENTION si le delimiteur comporte des quote alors les variables d'environnement ne s'expand pas,
//sinon on utilise ft_ifexpand sur chaque newline de heredoc.


