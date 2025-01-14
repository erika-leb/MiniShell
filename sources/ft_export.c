#include "../minishell.h"

//getcwd permet d'obtenir PWD meme sans environnement ?

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
static t_env *ft_display(char **env, char **args)
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
        return (ft_bbsort(head), ft_printexport(head), ft_freelexport(head), NULL);

    //Si on est la c'est qu'on va ajouter au moins une variable (args != NULL)

    ft_freelexport(head);
    return (head);
}


//Il faudrait proteger cette fonction pour eviter les erreurs chelous
static char **ft_ltoa(t_env *head)
{

    size_t  count;
    size_t  i;
    size_t  len;
    t_env   *current;
    char    **array;
    char    *buffer;

    if (!head)
        return (NULL);//maybe useless
    current = head;
    count = 0;
    //Calcul du nombre de variables d'env
    while (current)
    {
        count++;
        current = current->next;
    }
    array = malloc((count + 1) * sizeof(char *));//ft_calloc(count + 1, sizeof(char *));//
    size_t init;
    init = count;
    while (init--)
        array[init] = NULL;//si je mets pas ca j'ai des valeurs j'ai une erreur : valeu rnon initialisee
    if (!array) {
        perror("malloc failed");//gc_cleaner
        return NULL;
    }
    // Remplir le tableau avec les chaînes "name=key"
    current = head;
    i = -1;
    while (++i < count - 1)
    {
        len = ft_strlen(current->name) + ft_strlen(current->key) + 2;// +1 pour '=' et +1 pour '\0'
        array[i] = malloc(len * sizeof(char));
        if (!array[i])
            printf(" ");//gc_cleaner
        buffer = array[i];
        strcpy(buffer, current->name);   //A MODIFIER pour ft_strncpy ou alors coder ft_strcpy
        if (current->key)
        {
            buffer[ft_strlen(current->name)] = '=';
            strcpy(buffer + ft_strlen(current->name) + 1, current->key);
        }
        current = current->next;
    }
    // Ajouter un pointeur NULL à la fin du tableau si je fais gc cleaner
    //array[count] = NULL;
    return array;
}


//voir excel vietdu91

//gcc -o ft_export sources/env_manager.c sources/ft_tokenize.c sources/parsing.c sources/ft_concat.c sources/str_manager.c sources/libft_a.c sources/libft_abis.c sources/ft_export_utils.c sources/ft_split_utils.c sources/ft_split.c sources/ft_ambig.c sources/ft_getenvv.c sources/ft_ifexpand.c sources/ft_export.c
//valgrind --leak-check=full ./ft_export "bonjour=\"ok=ok\""

//zzzyzz' ='"boloss"       donne      `zzzyzz =boloss': not a valid identifier        En gro speut pas y avoir d'espace entre le nom et le '=' (apres concatenation).
//Il faut faut faire un parser pour gerer tous ces cas. Autre cas impossible : le nom de la variable ne peut pas commencer par =. Exemple "="hello=5.
//. ne peut pas etre present dans le name. 
int main(int argc, char *argv[], char *env[])
{
    t_env *lst;
    char **adder;
    int  j;

    if (argc == 1)
        return(ft_display(env, NULL), 0);
    lst = NULL;
    j = -1;
    while (env[++j])
    {
        adder = ft_calloc(2 + 1, sizeof(char *));
        adder[0] = ft_cut(env[j], '=', 0);
        adder[1] = ft_cut(env[j], '=', 1);
        lst = ft_addenvnode(lst, adder[0], adder[1]);
        ft_freesplit(adder, 3);
    }
    j = 0;//car argv[0] est le nom du prg, mais a voir si c adapte au code final.
    while (argv[++j])
    {
        //J'ai aussi remarque que si le user ecrit bonjour="cava \"oui\" et toi"  alors ca donne bonjour="cava \"oui\" et toi"
        //                                                                            au lieu de bonjour="cava \oui\ et toi"
        //Je pense que bash concatene deja les argv donc pour voir comment se comporte reellement ft_exit dans minishell il faut
        //le tester en grandeur nature.
        //A l'affichage (quand je fais ft_display(env, NULL)) il faudra juste ajouter des \ devant les $ et les ".
        adder = ft_calloc(2 + 1, sizeof(char *));//gc cleaner ??
        adder[0] = ft_cut(ft_concat(ft_ifexpand(argv[j], 0, 0), -1, 0, 0), '=', 0);
        adder[1] = ft_cut(ft_concat(ft_ifexpand(argv[j], 0, 0), -1, 0, 0), '=', 1);
        lst = ft_addenvnode(lst, adder[0], adder[1]);

        //NB : quand on aura fini export il faudra reecrire ft_getenvv car a partir de mtn on travaille plus avec l'environnement bash
        //mais avec notre tableau envv ??

        ft_freesplit(adder, 3);
    }

    char **array;
    
    // array = NULL;
    array = ft_ltoa(lst);
    j = -1;
    while (array[++j])
    {
        printf("%s\n", array[j]);
        free(array[j]);
    }
    free(array);
    //ft_freesplit(array, 256);

    ft_freelexport(lst);
    //Apres tri export(NULL) les minuscules sont bien en dernier ??
    //Retirer la variable _ dans la structure env de minishell (ca n'apparait pas dans bash --posix) ?? Sur ?? Car ca apparait dans bash --posix

    
    return 0;
}
