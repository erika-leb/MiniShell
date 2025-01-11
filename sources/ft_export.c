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

//gcc -o ft_export sources/str_manager.c sources/libft_a.c sources/libft_abis.c sources/ft_export_utils.c sources/ft_split_utils.c sources/ft_split.c sources/ft_ambig.c sources/ft_getenvv.c sources/ft_ifexpand.c sources/ft_export.c
//./ft_export
int main(int argc, char *argv[], char *env[])
{
    t_env *head;
    char *tmp1;
    char *tmp2;
    int  j;

    if (argc == 1)
        return(ft_export(env, NULL), 0);

    char **adder;

    j = 0;//on commence a argv[1]
    while (argv[++j])
    {

        //Continuer avec GPT

        //2) concat (apres une etape interm ou on cree un tableau de chaine de carac [chaine, NULL])
        //NB : quand on fait des tests on remarque que si j'ecris bonjour'='cava on obtient bonjour=cava,
        //et c'est surement parce que bash s'occupe deja de concatener argv quand on l'envoie dans ft_exit.
        //Hors dans minishell on passe pas par le bash mais c le user qui ecrit directement dans line.
        //3) split (avec le nouveau separateur '=' (hors sq dq))
        adder = ft_split(ft_ifexpand(argv[1], 0, 0), 0, 0);//Il faut changer le sep !!
        printf ("%s", adder[0]);
        if (adder[1])
            printf("(sep) %s\n", adder[1]);
        else
            printf("\n");
        //Apres avoir fait 1) 2) et 3) j'arime le tout a env (qui doit etre une structure ou une static char **)

        ft_freesplit(adder, 2);//apres separation de ce qui est avant et apres =, on free les 2 chaines
    }
    //NB : si je fais "export HELLO=5" puis HELLO alors ca change pas la valeur de HELLO car elle n'a pas de cle.
    //On se sert de la liste chainee cree pour voir si le name HELLO existe deja. Si ca existe pas alors je l'ajoute
    //a la fin de env (qui doit etre remalloc)

    //Apres tri export(NULL) les minuscules sont bien en dernier ??

    //NB expand et concat la var d'env avant de l'envoyer dans le pool de variables : export HELLO'='$HOME
    //ft_ifexpand et ft_concat ont l'air adapte car dans ifexpand on entre jamais dans les cas speciaux vu qu'il faut pas etre
    //dans des quotes et faut un espace "< ", ">> " etc

    //Bizarrement dans export si y'a un $ j'ai un \ devant pour signifier que c'est un caractere special (pas dans env)

    //tej la variable _ dans la structure env de minishell.

    
    return 0;
}
