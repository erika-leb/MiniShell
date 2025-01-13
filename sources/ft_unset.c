#include "../minishell.h"

//Demander quels sont les pieges a eviter

//bizarrement unset et export n'ont pas besoin de PATH pour fonctionner.
//Comme tous les autres built-in je pense.

//Je crois que pour unset si le name n'existe pas il se passe rien. Si par exemple je fais unset boloss=5 (sachant que boloss est une var d'env)
//alors unset considere que le name = "boloss=5" et donc il supprime rien

//NB : Pour unset il faut en fait s'assurer que si je supprime toutes les var d'env alors ca remet OLDPWD, PWD et SHLVL.