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

//Y'a rien a faire juste un affichage et message d'erreur si y'a des arguments.


//Il faudrait proteger cette fonction pour eviter les erreurs chelous
//env_manager
char **ft_ltoa(t_env *head)
{
    if (!head)
        return NULL;//maybe useless

    size_t count;
    t_env *current;

    // Calcul du nombre d'éléments
    current = head;
    count = 0;
    while (current)
    {
        count++;
        current = current->next;
    }

    char **array;
    array = ft_calloc(count + 1, sizeof(char *)); // Utilisation de calloc pour éviter les valeurs non initialisées
    // char **array;
    // array = malloc((count + 1) * sizeof(char *));
    // size_t init;
    // init = 0;
    // while (init < count)
    // {
    //     array[init] = NULL;//meme en mettant ca j'ai des valeurs non initialisees
    //     init++;
    // }
    if (!array)
    {
        perror("malloc failed");//gc_cleaner
        return NULL;
    }

    current = head;
    size_t i;

    i = 0;
    while (i < count)
    {
        size_t len = ft_strlen(current->name) + (current->key ? ft_strlen(current->key) : 0) + 2;
        array[i] = malloc(len * sizeof(char));
        if (!array[i])
        {
            //gc_malloc
            // perror("malloc failed");
            // while (i-- > 0) free(array[i]);
            // free(array);
            // return NULL;
        }
        strcpy(array[i], current->name);//A MODIFIER pour ft_strncpy ou alors coder ft_strcpy
        if (current->key)
        {
            ft_strcat(array[i], "=");//A MODIFIER pour ft_strncpy ou alors coder ft_strcat
            ft_strcat(array[i], current->key);//A MODIFIER pour ft_strncpy ou alors coder ft_strcat
        }
        // printf("%s\n", array[i]);// Debug temporaire
        current = current->next;
        i++;
    }
    return array;
}

void ft_freetab(char **array)
{
    int j;

    j = -1;
    while (array[++j])
        free(array[j]);
    free(array);

}

//zzzyzz' ='"boloss"       donne      `zzzyzz =boloss': not a valid identifier        En gro speut pas y avoir d'espace entre le nom et le '=' (apres concatenation).
//Il faut faut faire un parser pour gerer tous ces cas. Autre cas impossible : le nom de la variable ne peut pas commencer par =. Exemple "="hello=5.
//. ne peut pas etre present dans le name.
//Attention si on fait export HELLO= ca equivaut a export HELLO=""
void    ft_exparser(char **argv)
{

}

void    ft_envparser(char **argv)
{
    int i;

    i = 0;
    while (argv[i])
    {
        //j'ai remarque si je fais export -n HELLO alors ca cree pas la variable HELLO
        //et meme de facon general il peut pas y avoir d'option. En effet si je fais export -l j'ai l'erreur:
        //export: usage: export [-fn] [name[=value] ...] or export -p.
        //indiquant qu'il ne peut y avoir d'autres options que -fn et -p.
        if (argv[i][0] = '-')
            printf("env: No option(s) allowed\n");//ft_write 
    }

    exit;
}

void ft_env(char **array, char **cmds)
{
    int i;

    i = 0;
    if (cmds[1])
    {//parsing
        printf("env: No option(s) or argument(s) allowed\n");//ft_write 
        exit (127);//ou 125 ?
    }
    while (array[i])
        printf("%s\n", array[i++]);//ft_write
}