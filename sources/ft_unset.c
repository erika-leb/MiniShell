#include "../minishell.h"

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

void    ft_unset(char **env, char **argv)
{//la boucle current doit etre enrobee dans la boucle argv car il faut le faire pour chaque argv a partir de cmds[1]
    t_env *head;
    t_env *current;
    t_env *previous;
    t_env *temp;//
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
                temp = current->next;
                if (previous)
                    previous->next = current->next;
                else
                    head = temp;
                free(current->name);
                free(current->key);
                free(current);
                current = temp;
            }
            else
            {
                previous = current;
                current = current->next;
            }
        }
        i++;
    }
    //a la fin on modifie env et on free l'ancienne version !!!!!!!
    //(Ici on le fait pas car env est de la stack et c le vrai env)
    ft_printexport(head);
    ft_freelexport(head);
    return (ft_printexport(head), ft_freelexport(head));
}

//gcc -o ft_unset sources/env_manager.c sources/ft_tokenize.c sources/parsing.c sources/ft_concat.c sources/str_manager.c sources/libft_a.c sources/libft_abis.c sources/ft_export_utils.c sources/ft_split_utils.c sources/ft_split.c sources/ft_ambig.c sources/ft_getenvv.c sources/ft_ifexpand.c sources/ft_export.c sources/ft_unset.c
//valgrind --leak-check=full ./ft_unset "bonjour=\"ok=ok\""
int main(int argc, char *argv[], char *env[])
{
    // //ft_export
    // char **array;

    // //Si cmd[1] est vide alors on fera ft_export(env, NULL);
    // //Dans ft_export si le deuxieme argument est un pointeur NULL alors ca marche tout seul
    // if (argc == 1)
    // {
    //     ft_export(env, NULL);
    //     return (0);
    // }
    // //Si cmd[1] non vide alors ft_export(env, argv);
    // array = ft_export(env, argv);
    // //afficher array /////////////////////
    // ft_env(array, NULL);
    // //////////////////////////////////////
    // ft_freetab(array);
    // return 0;
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ft_unset(env, argv);
    return 0;
}