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


//void ft_filltoa(char **array, t_env *current, size_t i)
t_env *ft_filltoa(char **array, t_env *current, size_t i)
{
    size_t  j;
    size_t  len;

    if (current->key)
        len = ft_strlen(current->name) + ft_strlen(current->key) + 2;
    else
        len = ft_strlen(current->name) + 2;
    array[i] = malloc(len * sizeof(char));
    j = -1;
    while (current->name[++j])
        array[i][j] = current->name[j];
    array[i][j] = '\0';//useless avec gc_calloc I guess
    if (current->key)
        ft_strcat(ft_strcat(array[i], "="), current->key);
    // printf("%s\n", array[i]);// Debug temporaire
    return (current->next);
}

//Il faudrait proteger cette fonction pour eviter les erreurs chelous
char **ft_ltoa(t_env *head)
{
    size_t  count;
    size_t  i;
    t_env   *current;
    char    **array;

    if (!head)
        return NULL;//maybe useless
    current = head;
    count = 0;//Erika utilise ft_arr_size(char **tab) pour calculer nb de chaines dans un tableau
    while (current)
    {
        count++;
        current = current->next;
    }
    array = ft_calloc(count + 1, sizeof(char *)); // Utilisation de calloc pour éviter les valeurs non initialisées
    current = head;
    i = 0;
    while (i < count)
        current = ft_filltoa(array, current, i++);
    return (array);
}

void ft_freetab(char **array)
{
    int j;

    j = -1;
    while (array[++j])
        free(array[j]);
    free(array);

}

//export +=5. Si la var d'env existe deja ca ajoute 5 a la suite.
//Le + doit se situer juste devant le =.

//De facon general il faut bien faire gaffe au comportement de minishell si le user s'amuse a mettre les options n'importe ou dans cmds[i]
int   ft_exparser(char *name_key)
{
    int i;

    //(il faudrait remplir le meme ft_write que ce qu'il y en dessous ?)
    if (!ft_isalpha(name_key[0]) && name_key[0] != '_')
        return (printf("export: FIRST LETTER not a valid identifier\n"));
    i = 0;//on check a partir du 2eme caractere c'est pourquoi on met i = 0.
    while (name_key[++i])
    {
        if (name_key[i] == '+' && name_key[i + 1] == '=')
        {
            //dummy = 1 pour que addenvnode on fasse une concatenation (useless I guess, autan tle faire dans addenvnode et envnode)
            break;
        }
        if (name_key[i] == '=')
            break;
        if (!ft_isalnum(name_key[i]) && name_key[i] != '_')
            return(printf("export: `name=key': not a valid identifier\n"));
    }
    return (0);
}

static char	*ft_strchr(char const *str, int c)
{
	unsigned int	i;
	char			char_c;

	i = 0;
	char_c = (char)c;
	while (str[i] != '\0')
	{
		if (str[i] == char_c)
			return ((char *)&str[i]);
		i++;
	}
	if (str[i] == char_c)
		return ((char *)&str[i]);
	return (NULL);
}

void ft_env(char **array, char **cmds)
{
    int i;

    if (cmds && cmds[1])//cmds && servait juste a le tester dans mon main
    {//parsing. cmds[0] = env     cmds[1...] = le reste ...
        printf("env: No option(s) or argument(s) allowed\n");//ft_write 
        exit (127);//ou 125 ?
    }
    i = 0;
    while (array[i])
    {
        if (ft_strchr(array[i],'='))
            printf("%s\n", array[i]);//ft_write
        i++;
    }
}