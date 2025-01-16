#include "../minishell.h"

//GOOD TO KNOW
    //getcwd permet d'obtenir PWD meme sans environnement ?
    //si je fais export adri (sans cle) et que je fais export alors ca apparait sans cle. Mais apres si je fais env
    //je vois pas apparaitre adrien
    //Dans bash --posix OLDPWD existe mais n'a pas de cle

//Dans notre minishell il peut etre judicieux de hardcod le chemin ou se situe notre bash
//le OLDPWD et mettre SHLVL a 1 puis on va l'incrementer si le user fait des ./minishell | ./minishell
//D'ailleurs le ./minishell | ./minishell est un hedge case a prendre en compte dans la partie exec d'erika
//Le souci est qu'on sait pas si le user va lancer le minishell dans goinfre, sgoinfre etc...)

//Demander quels sont les pieges a eviter

//Je crois que pour unset si le name n'existe pas il se passe rien. Si par exemple je fais unset boloss=5 (sachant que boloss est une var d'env)
//alors unset considere que le name = "boloss=5" et donc il supprime rien

//NB : Pour unset il faut en fait s'assurer que si je supprime toutes les var d'env alors ca remet OLDPWD, PWD et SHLVL? A priori nope,
//on a pas a gerer ca vu que env -i ne peut se faire que hors du minishell.

//EN REVANCHE, au debut de minishell je dois verifier que si env est vide (ou NULL?) alors je vais chercher PWD, je simule (?)  SHLVL et 
//j'incorpore OLPWD sans cle

//NB : pour unset on pourra facilement supprimer 1 ou plsr var en utilisant la meme logique que export de la chaine de caractere :
//je créé une liste temporaire, je supprime le(s) noeud(s) souhaité(s) et je recréé un nouveau tableau envv mis a jour.
//En oubliant pas que si je supprime tout il doit quand meme y avoir SHLVL OLDPWD et PWD.

//tranformer env en liste chainee. remove le noeud name (si y'en a pas on fait rien). je retransforme en envv et je remplace l'ancien env.

//A TESTER avec un main()
void    ft_unset(char **env, char **argv)
{//la boucle current doit etre enrobee dans la boucle argv car il faut le faire pour chaque argv a partir de cmds[1]
    t_env *head;
    t_env *current;
    t_env *previous;
    int    i;

    if (!argv[1])
        return ;
    head = NULL;
    i = -1;
    while (env[++i])
        ft_adder(&head, env[i]);
    current = head;
    previous = NULL;
    i = 1;//on commence a cmds[1]
    while (argv[i])
    {
        while (current)
        {
            if (strcmp(current->name, argv[i]) == 0)
            {
                // Nœud trouvé, suppression
                if (previous)
                {
                    // Le nœud n'est pas le premier, on ajuste le lien du nœud précédent
                    previous->next = current->next;
                }
                else
                {
                    // Le nœud est le premier, on déplace le head
                    head = current->next;
                }
                // Libération du nœud
                free(current->name);
                free(current->key);
                free(current);
            }
            // Avancer dans la liste
            previous = current;
            current = current->next;
        }
        i++;
    }
    //a la fin on modifie env et on free l'ancienne version !!!!!!! (Ici on le fait pas car env est de la stack et c le vrai env)
    ft_printexport(head);
    ft_freelexport(head);
}