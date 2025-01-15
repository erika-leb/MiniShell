#include "../minishell.h"

static void ft_printexport(const t_env *head)
{
    const t_env *current = head;
    //Tout mettre dans un buffer, comme pour l'erreur de ft_exit
    while (current) {
        if (current->key)
            printf("export %s=\"%s\"\n", current->name, current->key);
        else
            printf("export %s\n", current->name);
        current = current->next;
    }
}

static void ft_freelexport(t_env *head)
{
    t_env *current = head;
    while (current) {
        t_env *temp = current;
        current = current->next;
        free(temp->name);
        free(temp->key);
        free(temp);
    }
}

//NB : quand on aura fini export il faudra reecrire ft_getenvv car a partir de mtn
//on travaille plus avec l'environnement bash
//mais avec notre tableau envv ??
static void ft_adder(t_env **head, char *str, int concat)
{
    char  **adder;

    adder = ft_calloc(2 + 1, sizeof(char *));//gc_cleaner
    if (!concat)
    {
        adder[0] = ft_cut(str, '=', 0);
        adder[1] = ft_cut(str, '=', 1);
    }
    else
    {
        adder[0] = ft_cut(ft_concat(ft_ifexpand(str, 0, 0), -1, 0, 0), '=', 0);
        adder[1] = ft_cut(ft_concat(ft_ifexpand(str, 0, 0), -1, 0, 0), '=', 1);
    }
    *head = ft_addenvnode(*head, adder[0], adder[1]);
    ft_freesplit(adder, 3);
    
}


//J'ai aussi remarque que si le user ecrit bonjour="cava \"oui\" et toi"  alors ca donne bonjour="cava \"oui\" et toi"
//                                                                            au lieu de bonjour="cava \oui\ et toi"
//Je pense que bash concatene deja les argv donc pour voir comment se comporte reellement ft_exit dans minishell il faut
//le tester grandeur nature.
//A l'affichage (quand je fais ft_export(env, NULL)) il faudra juste ajouter des \ devant les $ et les ".

//a chaque fois que je fais cd je peux directement fair appel a ft_export
//pour modifier OLPWD et PWD
static char **ft_export(char **env, char **argv)
{
    // Gerer le cas ou env est NULL : env -i ./minishell (voir bloc note)
    t_env *head;
    char  **adder;
    int   i;

    head = NULL;
    i = -1;
    while (env[++i])
        ft_adder(&head, env[i], 0);
    if (!argv)
        return (ft_bbsort(head), ft_printexport(head), ft_freelexport(head), NULL);
    i = 0;//car argv[0] est le nom du prg, mais a voir si c adapte au code final.
    //Il faudra le changer quand on passera a cmd[i]
    while (argv[++i])
        ft_adder(&head, argv[i], 1);
    adder = ft_ltoa(head);
    // ft_printexport(head);
    ft_freelexport(head);
    //Retirer la variable _ dans la structure env de minishell
    //(ca n'apparait pas dans bash --posix) ??
    //Sur ?? Car ca apparait dans bash --posix
    return (adder);
}

//voir excel vietdu91

//gcc -o ft_export sources/env_manager.c sources/ft_tokenize.c sources/parsing.c sources/ft_concat.c sources/str_manager.c sources/libft_a.c sources/libft_abis.c sources/ft_export_utils.c sources/ft_split_utils.c sources/ft_split.c sources/ft_ambig.c sources/ft_getenvv.c sources/ft_ifexpand.c sources/ft_export.c
//valgrind --leak-check=full ./ft_export "bonjour=\"ok=ok\""

 
int main(int argc, char *argv[], char *env[])
{
    char **array;

    //Si cmd[1] est vide alors on fera ft_export(env, NULL);
    //DEJA GERE DANS ft_export !!!
    if (argc == 1)
    {
        ft_export(env, NULL);
        return (0);
    }

    //Si cmd[1] non vide alors ft_export(env, argv);
    array = ft_export(env, argv);
    //afficher array /////////////////////
    // int i;
    // i = 0;
    // while (array[i])
    //     printf("%s\n", array[i++]);
    //////////////////////////////////////
    ft_freetab(array);

    
    return 0;
}
