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
        new_node->key = ft_strdup_(key);//else if dernier terme de name est un '=' alors new_node->key = "\0";
    else
        new_node->key = NULL;
    new_node->next = NULL;
    return (new_node);
}

// Fonction pour ajouter un nœud à la fin de la liste
//Autrement dit : on cherche s'il n'existe pas deja une var d'env nommée adder[0] avec strcmp. Si oui, alors on remplace sa clé
//par la nouvelle clé (sauf si elle est NULL). Si la var d'env n'existe pas on l'ajoute a envv (on ecrase envv et on remalloc
//une liste mise a jour).
t_env *ft_addenvnode(t_env *head, char *name, char *key)
{
    t_env *current;
    
    //S'il peut y avoir un + dans name (par exemple HELLO+ car j'ai voulu faire HELLO+=5) alors on le tej ?
    if (!head)
        return (ft_envnode(name, key));
    current = head;
    while (current)
    {
        if (ft_strcmp(current->name, name) == 0)
        {
            // Si un nœud avec le même "name" est trouvé, mettre à jour "key"
            if (key)
            {
                //Si y'a un '+' en derniere position de name alors on concatene (strlcat ?)
                //(on le fait dans key car si y'a pas de key on a rien a faire)

                free(current->key);
                current->key = ft_strdup_(key);
            }
            return (head);
        }
        // Des qu'on a tout checke on arrete et on ajoute simplement le noeud a la suite
        if (!current->next)
            break;
        current = current->next;
    }
    // Si aucun nœud avec le même "name" n'a été trouvé, ajouter un nouveau nœud à la fin
    current->next = ft_envnode(name, key);
    return head;
}

char *ft_cut(const char *src, char delim, int is_end)
{
    // static char *result;
    char *result;
    size_t i;
    
    i = 0;
    while (src[i] && src[i] != delim)
        i++;
    if (is_end)
    {
        if (src[i] == delim)
            return (ft_strdup_(src + i + 1));
        return (NULL); // Si y'a pas de delimiteur alors key est un pointeur NULL
    }
    else
    {
        // Si on veut la partie avant le délimiteur
        result = (char *)malloc(i + 1);
        if (!result)
            return (NULL);//gc_cleaner ?
        ft_strncpy(result, src, i);
        result[i] = '\0';
        return (result);
    }
}

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
    int swapped;
    t_env *ptr1;
    t_env *lptr;

    if (head == NULL)
        return;
    swapped = 1;
    lptr = NULL;
    while (swapped)
    {
        swapped = 0; // Réinitialiser le flag à 0 à chaque nouveau tri
        ptr1 = head;
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