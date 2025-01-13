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
//void ft_display(char **env, char **args)
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

    //Si on est la c'est qu'on va ajouter au moins une variable (args != NULL)

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
        //J'ai aussi remarque que si le user ecrit bonjour="cava \"oui\" et toi"  alors ca donne bonjour="cava \"oui\" et toi"
        //                                                                            au lieu de bonjour="cava \oui\ et toi"
        //En gros il faut pas concatener ce qu'il y a apres le = (et entre les guillemets simples ou doubles). Mais a l'affichage
        //(quand je fais ft_export(env, NULL)) il faudra juste ajouter des \ devant les $ et les ".

        adder = ft_calloc(2 + 1, sizeof(char *));//gc cleaner
        adder[0] = ft_cut(ft_concat(ft_ifexpand(argv[j], 0, 0), -1, 0, 0), '=', 0);
        adder[1] = ft_cut(ft_concat(ft_ifexpand(argv[j], 0, 0), -1, 0, 0), '=', 1);
        printf("%s", adder[0]);
        if (adder[1])
            printf (" (=) %s\n", adder[1]);
        else
            printf("\n");

        //Il faut maintenant arimer le tout a env (qui doit etre une structure ou une static char **).
        //Le mieux c'est de faire en sorte que envv reste un tableau de chaine de caractere comme env.
        //Donc ici il faut cree un noeud (pour chaque couple adder[0] et adder[1]), transformer envv temporairement en
        //liste chainee et comparer le noeud adder[0]/adder[1] avec chaque noeud envv de la facon suivante :

        //Si adder[1] est NULL alors il me suffit de verifier qu'il n'existe pas deja une var d'env du meme nom (avec strcmp).
        //Si c'est le cas on ne fait rien. Sinon on l'ajoute a envv.
        //Si adder[1] n'est pas NULL alors on verifie s'il existe deja une var d'env du meme nom. Si c'est le cas on remplace sa cle,
        //sinon on l'ajoute a envv.
        //Autrement dit : on cherche s'il n'existe pas deja une var d'env nommée adder[0] avec strcmp. Si oui, alors on remplace sa clé
        //par la nouvelle clé (sauf si elle est NULL). Si la var d'env n'existe pas on l'ajoute a envv (on ecrase envv et on remalloc
        //une liste mise a jour).

        //NB : quand on aura fini export il faudra reecrire ft_getenvv car a partir de mtn on travaille plus avec l'environnement bash
        //mais avec notre tableau envv

        //NB : Pour unset il faut en fait s'assurer que s'il manque OLDPWD PWD ou SHLVL on le remet. Ou plus simplement que unset ne peut pas supprimer SHLVL (a verifier sur bash --posix).

        //NB : pour unset on pourra facilement supprimer 1 ou plsr var en utilisant la meme logique de la chaine de caractere :
        //je créé une liste temporaire, je supprime le(s) noeud(s) souhaité(s) et je recréé un nouveau tableau envv mis a jour.
        //En oubliant pas que si je supprime tout il doit quand meme y avoir SHLVL OLDPWD et PWD.

        ft_freesplit(adder, 3);
    }

    //Apres tri export(NULL) les minuscules sont bien en dernier ??

    //Retirer la variable _ dans la structure env de minishell (ca n'apparait pas dans bash --posix).

    
    return 0;
}
