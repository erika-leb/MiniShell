#include "../minishell.h"

// static char	*ft_strdup_(char const *str)
// {
// 	char	*dest;
// 	int	i;

// 	i = -1;
// 	dest = malloc(ft_strlen(str) + 1);
// 	if (dest == NULL)
// 		return (NULL);//gc_cleaner
// 	while (str[++i])
// 		dest[i] = str[i];
// 	dest[i] = '\0';
// 	return (dest);
// }

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

// char *ft_strncpy(char *dest, const char *src, size_t n)
// {

//     size_t i;

//     i = -1;
//     while (src[++i] && i < n)
//         dest[i] = src[i];
//     while (i++ < n)
//         dest[i] = '\0';
//     return dest;
// }

static char *ft_cut(const char *src, char delim, int is_end)
{
    char *result;
    size_t i = 0;

    // Trouver la position du délimiteur
    while (src[i] && src[i] != delim)
        i++;
    if (is_end) {
        // Si on veut la partie après le délimiteur
        if (src[i] == delim)
            return ft_strdup_(src + i + 1); // Copier la chaîne après le délimiteur
        return NULL; // Une variable d'environnement sans le contenu
    } else {
        // Si on veut la partie avant le délimiteur
        result = (char *)malloc(i + 1);
        if (!result)
            return NULL;
        ft_strncpy(result, src, i); // Copier la partie avant le délimiteur
        result[i] = '\0';
        return result;
    }
}

// Fonction pour échanger deux nœuds
void swap_nodes(t_env *node1, t_env *node2) {
    char *temp_name = node1->name;
    char *temp_key = node1->key;

    node1->name = node2->name;
    node1->key = node2->key;
    
    node2->name = temp_name;
    node2->key = temp_key;
}

void bubble_sort(t_env *head) {
    int swapped = 1; // On initialise `swapped` à 1 pour entrer dans la boucle
    t_env *ptr1;
    t_env *lptr = NULL;

    if (head == NULL) {
        return;
    }

    // Tant qu'il y a eu un échange dans la dernière passe, on continue
    while (swapped) {
        swapped = 0; // Réinitialiser le flag à 0 à chaque nouvelle passe
        ptr1 = head;

        // Comparer chaque nœud avec le suivant
        while (ptr1->next != lptr) {
            if (strcmp(ptr1->name, ptr1->next->name) > 0) {
                swap_nodes(ptr1, ptr1->next);
                swapped = 1; // Si on a échangé, on met `swapped` à 1
            }
            ptr1 = ptr1->next;
        }

        lptr = ptr1; // Marquer le dernier élément comme déjà trié
    }
}

//env est soit trie et affiche, soit modifie.
//a chaque fois que je fais cd je peux directement fair appel a ft_export
//pour modifier OLPWD et PWD
void ft_export(char **env, char *arg)
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
    if (!arg)
        return (bubble_sort(head), print_list(head), free_list(head));

    //Si on est la c'est qu'on va ajouter une variable (arg != NULL)

    //GOOD TO KNOW
    //si je fais export kiwi="fraise" donne le meme resultat que kiwi=fraise
    //si je fais export kiwi="fraise" puis je fais export kiwi alors comme kiwi existe deja et que j'ai pas attribue de
    //nouvelle cle, alors il se passe rien
    //si je fais export adri (sans cle) et que je fais export alors ca apparait sans cle. Mais apres si je fais env
    //je vois pas apparaitre adrien
    //Dans bash --posix OLDPWD existe mais n'a pas de cle
    free_list(head);
}

//gcc -o ft_export sources/str_manager.c sources/libft_a.c sources/ft_export.c
//./ft_export
int main(int argc, char *argv[], char *env[])
{
    t_env *head;
    char *tmp1;
    char *tmp2;
    int   i;

    

    //Le cas ou j'ecris juste export sans creer de nouvelle variable
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
    bubble_sort(head);
    print_list(head);
    free_list(head);
    ///////////////////////

    return 0;
}
