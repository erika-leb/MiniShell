#include "../minishell.h"

//Si j'essaie de mettre une option mettre un message d'erreur (ex. no options allowed) puis
//exit avec le code 125

//Dans notre minishell il peut etre judicieux de hardcod le chemin ou se situe notre bash
//le OLDPWD et mettre SHLVL a 1 puis on va l'incrementer si le user fait des ./minishell | ./minishell
//D'ailleurs le ./minishell | ./minishell est un hedge case a prendre en compte dans la partie exec d'erika
//Le souci est qu'on sait pas si le user va lancer le minishell dans goinfre, sgoinfre etc...)

//Est-ce que les var d'env sont ranges dans un certain ordre ?
//Quand je fais export, j'ai du mal a comprendre ou je dois placer une nouvelle variable cree avec export.