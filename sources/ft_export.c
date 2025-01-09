#include "../minishell.h"

//penser a la possibilite de lancer le built-in export sans arguments : ca donne le meme resultat que env
//mais presente un peu differement : range dans l'ordre alphabetique, et les maj avant les min
//(on peut faire un bubble sort)

//si je fais export kiwi="fraise" donne le meme resultat que kiwi="fraise"

//si je fais export kiwi="fraise" puis je fais export kiwi     comme kiwi existe deja et que j'ai pas attribue de
//nouvelle cle, alors il se passe rien

//si je fais export adri (sans cle) et que je fais export alors ca apparait sans cle. Mais apres si je fais env
//je vois pas apparaitre adrien

//Dans bash --posix y'a pas de OLDPWD si je fais env -i bash --posix ??

#include <string.h>//Pour quelle fonction ? Useless dans la vf ?

static char	*ft_strdup_(char const *str)
{
	char	*dest;
	int	i;

	i = -1;
	dest = malloc(ft_strlen(str) + 1);
	if (dest == NULL)
		return (NULL);//gc_cleaner
	while (str[++i])
		dest[i] = str[i];
	dest[i] = '\0';
	return (dest);
}

//Si dans l'argument d'export y'a pas de = alors key == NULL quand je cree la variable
static t_env *ft_envnode(const char *name, const char *key)
{
    t_env *new_node;
    
    new_node = (t_env *)malloc(sizeof(t_env));
    if (!new_node)
    {
        perror("Erreur d'allocation mémoire");//gc_cleaner
        exit(EXIT_FAILURE);
    }
    new_node->name = ft_strdup_(name);
    if (key)
        new_node->key = ft_strdup_(key);
    else
        new_node->key = NULL;
    new_node->next = NULL;
    return (new_node);
}

// Fonction pour ajouter un nœud à la fin de la liste
static t_env *ft_addenvnode(t_env *head, const char *name, const char *key)
{
    t_env *current;

    if (!head)
        return (ft_envnode(name, key));
    current = head;
    while (current->next)
        current = current->next;
    current->next = ft_envnode(name, key);
    return (head);
}

// Fonction pour afficher la liste chaînée
void print_list(const t_env *head)
{
    const t_env *current = head;
    while (current) {
        printf("Nom : %s, Valeur : %s\n", current->name, current->key ? current->key : "NULL");
        current = current->next;
    }
}

// Fonction pour libérer la mémoire allouée à la liste chaînée
void free_list(t_env *head)
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

//Verifier qu'on a pas impacte env !!! Vu qu'on fait tmp = src;
static char *ft_strtok(char *src, char delim, int is_begin)
{
    static char *tmp;
    static char *tmp2;
    //delim = '=';
    //if (is_begin) alors on doit prendre ce qui vient apres le delim.
    //Sinon on prend ce qui vient avant.

    tmp = strcpy(src);//strcpy
    while (src[i] && sr[i] != delim)
        i++;//sert a trouver le = et retourner ce qu'il y a avant ou apres, au besoin
    ft_insert(tmp, i, '\0');
    if (!is_begin)
        return (tmp);
    //Si on arrive ici c'est qu'on veut ce qu'il y a apres le =.
    //Il faut donc ft_erase_substr jusqu'au =.
    //ATTENTION QUE PASA SI Y'A PAS DE = ???
    tmp2 = strcpy(src);
    ft_erase_substr(tmp2, *i, tmp);
    return (tmp2);
}

//gcc -o ft_export sources/str_manager.c sources/libft_a.c sources/ft_export.c
//./ft_export
int main(int argc, char *argv[], char *env[])
{
    t_env *head;
    int   i;

    // Gerer le cas ou env est NULL (voir bloc note)
    //Valable if (*env)
    head = NULL;
    i = 0;
    while (env[i])
    {
        char *name = strtok(env[i], "=");
        char *key = strtok(NULL, "");
        head = ft_addenvnode(head, name, key);
        i++;
    }
    print_list(head);
    free_list(head);
    return 0;
}
