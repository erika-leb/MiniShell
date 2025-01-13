#include "../minishell.h"

//Si dans l'argument d'export y'a pas de = alors key == NULL quand je cree la variable
t_env *ft_envnode(const char *name, const char *key)
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
t_env *ft_addenvnode(t_env *head, const char *name, const char *key)
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

char *ft_cut(const char *src, char delim, int is_end)
{
    static char *result;
    //char *result;
    size_t i;
    
    i = 0;
    // Trouver la position du délimiteur
    while (src[i] && src[i] != delim)
        i++;
    if (is_end) {
        // Si on veut la partie après le délimiteur
        if (src[i] == delim)
            return ft_strdup_(src + i + 1); // Copier la chaîne après le délimiteur
        return NULL; // Une variable d'environnement sans le contenu
    }
    else
    {
        // Si on veut la partie avant le délimiteur
        result = (char *)malloc(i + 1);
        if (!result)
            return NULL;//gc_cleaner ?
        ft_strncpy(result, src, i); // Copier la partie avant le délimiteur. Risque de memory overlapping ? On s'en balek
        result[i] = '\0';
        return (result);
    }
}

// Fonction pour échanger deux nœuds
void ft_swapnodes(t_env *node1, t_env *node2)
{
    char *temp_name = node1->name;
    char *temp_key = node1->key;

    node1->name = node2->name;
    node1->key = node2->key;
    node2->name = temp_name;
    node2->key = temp_key;
}

void ft_bbsort(t_env *head)
{
    int swapped = 1; // On initialise `swapped` à 1 pour entrer dans la boucle
    t_env *ptr1;
    t_env *lptr = NULL;

    if (head == NULL)
        return;
    // Tant qu'il y a eu un échange dans la dernière passe, on continue
    while (swapped)
    {
        swapped = 0; // Réinitialiser le flag à 0 à chaque nouvelle passe
        ptr1 = head;
        // Comparer chaque nœud avec le suivant
        while (ptr1->next != lptr)
        {
            if (ft_strcmp(ptr1->name, ptr1->next->name) > 0)
            {
                ft_swapnodes(ptr1, ptr1->next);
                swapped = 1; // Si on a échangé, on met `swapped` à 1
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1; // Marquer le dernier élément comme déjà trié
    }
}