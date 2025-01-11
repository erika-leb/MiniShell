#include "../minishell.h"

// Fonction pour afficher la liste chaînée
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

// Fonction pour libérer la mémoire allouée à la liste chaînée
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

//env est soit trie et affiche, soit modifie.
//a chaque fois que je fais cd je peux directement fair appel a ft_export
//pour modifier OLPWD et PWD
void ft_export(char **env, char **args)
{
    // Gerer le cas ou env est NULL : env -i ./minishell (voir bloc note)

    t_env *head;
    char *tmp1;
    char *tmp2;
    int   i;

    head = NULL;
    i = -1;
    while (env[++i])
    {
        tmp1 = ft_cut(env[i], '=', 0);
        tmp2 = ft_cut(env[i], '=', 1);
        head = ft_addenvnode(head, tmp1, tmp2);
        free(tmp1);
        free(tmp2);
    }
    //Le cas ou j'ecris juste export sans creer de nouvelle variable
    if (!args)
        return (ft_bbsort(head), ft_printexport(head), ft_freelexport(head));

    //Si on est la c'est qu'on va ajouter une variable (args != NULL)

    //GOOD TO KNOW
    //si je fais export kiwi="fraise" donne le meme resultat que kiwi=fraise, qui est pareil que kiwi='fraise', donc il suffit de concat pour tej les quotes
    //avant de creer la variable.
    //si je fais export kiwi="fraise" puis je fais export kiwi alors comme kiwi existe deja et que j'ai pas attribue de
    //nouvelle cle, alors il se passe rien
    //si je fais export adri (sans cle) et que je fais export alors ca apparait sans cle. Mais apres si je fais env
    //je vois pas apparaitre adrien
    //Dans bash --posix OLDPWD existe mais n'a pas de cle
    //On peut export autant de variables qu'on veut a la fois : export AAA=% BBB CCC=AHAH
    ft_freelexport(head);
}

//gcc -o ft_export sources/str_manager.c sources/libft_a.c sources/libft_abis.c sources/ft_export_utils.c sources/ft_export.c
//./ft_export
int main(int argc, char *argv[], char *env[])
{
    t_env *head;
    char *tmp1;
    char *tmp2;
    int   i;

    //if useless je peux juste laisser argv car un pointeur NULL peut etre represente par un pointeur de pointeur
    if (argc == 1)
        ft_export(env, NULL);
    else
        ft_export(env, argv);
    return 0;
}
