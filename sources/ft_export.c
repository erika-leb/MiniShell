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
    //si je fais export adri (sans cle) et que je fais export alors ca apparait sans cle. Mais apres si je fais env
    //je vois pas apparaitre adrien
    //Dans bash --posix OLDPWD existe mais n'a pas de cle
    //On peut export autant de variables qu'on veut a la fois : export AAA=% BBB CCC=AHAH
    ft_freelexport(head);
}

//voir excel vietdu91


//gcc -o ft_export sources/ft_tokenize.c sources/parsing.c sources/ft_concat.c sources/str_manager.c sources/libft_a.c sources/libft_abis.c sources/ft_export_utils.c sources/ft_split_utils.c sources/ft_split.c sources/ft_ambig.c sources/ft_getenvv.c sources/ft_ifexpand.c sources/ft_export.c
//valgrind --leak-check=full ./ft_export "bonjour=\"ok=ok\""

//zzzyzz' ='"boloss"       donne      `zzzyzz =boloss': not a valid identifier        En gro speut pas y avoir d'espace entre le nom et le '=' (apres concatenation).
//Il faut faut faire un parser pour gerer tous ces cas. Autre cas impossible : le nom de la variable ne peut pas commencer par =. Exemple "="hello=5.  
int main(int argc, char *argv[], char *env[])
{
    t_env *head;
    char *tmp1;
    char *tmp2;
    int  j;

    if (argc == 1)
        return(ft_export(env, NULL), 0);

    char **adder;//n'est pas malloc, on a pas besoin de le free
    char **new_node;

    j = 0;
    while (argv[++j])
    {

        //Il faut trouver un moyen pour ft_splitter s'arrete au 1er '='. ft_concat est parfaitement place
        //En effet si jamais j'ecris ./ft_export bonjour'='"$HOME =cava"       j'obtiens     ./ft_export bonjour=$HOME =cava et le split bug

        //J'ai aussi remarque que si le user ecrit bonjour="cava \"oui\" et toi"  alors ca donne bonjour="cava \"oui\" et toi"
        //                                                                            au lieu de bonjour="cava \oui\ et toi"
        //Il faudrait remasteriser le ft_concat (en ft_concate) si j'ai le temps, en introduisant \" s'il concat un ". On peut par exemple
        //ajouter une variable dq tel que si result_i[to_erase] = '\"' alors dq = !dq et apres avoir erase on insere \" (je crois que le buffer
        //est de taille 20 000 donc ca pose pas de pb).
        //Il faudrait aussi parcourir tout le resultat et ajouter un \ si je mets un $ (c'est un pb que j'ai remarque aussi).

        adder = ft_splitter(ft_concat(ft_ifexpand(argv[j], 0, 0), -1, 0, 0), 0);
        printf ("%s (=) %s\n", adder[0], adder[1]);


        //Il faut maintenant arimer le tout a env (qui doit etre une structure ou une static char **) : FUSIONNER ft_export et main

        ft_freesplit(adder, 3);//apres separation de ce qui est avant et apres =, on free les 2 chaines
    }
    //NB : si je fais "export HELLO=5" puis HELLO alors ca change pas la valeur de HELLO car elle n'a pas de cle.
    //On se sert de la liste chainee cree pour voir si le name HELLO existe deja. Si ca existe pas alors je l'ajoute
    //a la fin de env (qui doit etre remalloc)

    //Apres tri export(NULL) les minuscules sont bien en dernier ??

    //Retirer la variable _ dans la structure env de minishell (ca n'apparait pas dans bash --posix).

    
    return 0;
}
