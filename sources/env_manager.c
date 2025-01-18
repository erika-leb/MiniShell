#include "../minishell.h"

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
    count = 0;
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

//De facon general il faut bien faire gaffe au comportement de minishell
//si le user s'amuse a mettre les options n'importe ou dans cmds[i] ?
int   ft_exparser(char *name_key)
{
    int i;

    //creer un buffer, remplir avec le 1er if, puis avec le dernier if et enfin tout envoyer (+return 1)
    if (!ft_isalpha(name_key[0]) && name_key[0] != '_')
        return (printf("export: FIRST LETTER not a valid identifier\n"));
    i = 0;//on check a partir du 2eme caractere c'est pourquoi on met i = 0.
    while (name_key[++i])
    {
        if (name_key[i] == '+' && name_key[i + 1] == '=')
            break;
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