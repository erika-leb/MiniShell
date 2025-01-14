#include "../minishell.h"

//Demander quels sont les pieges a eviter

//bizarrement unset et export n'ont pas besoin de PATH pour fonctionner.
//Comme tous les autres built-in je pense.

//Je crois que pour unset si le name n'existe pas il se passe rien. Si par exemple je fais unset boloss=5 (sachant que boloss est une var d'env)
//alors unset considere que le name = "boloss=5" et donc il supprime rien

//NB : Pour unset il faut en fait s'assurer que si je supprime toutes les var d'env alors ca remet OLDPWD, PWD et SHLVL.

//NB : pour unset on pourra facilement supprimer 1 ou plsr var en utilisant la meme logique que export de la chaine de caractere :
//je créé une liste temporaire, je supprime le(s) noeud(s) souhaité(s) et je recréé un nouveau tableau envv mis a jour.
//En oubliant pas que si je supprime tout il doit quand meme y avoir SHLVL OLDPWD et PWD.