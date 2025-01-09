#include "../minishell.h"

//penser a la possibilite de lancer le built-in export sans arguments : ca donne le meme resultat que env
//mais presente un peu differement : range dans l'ordre alphabetique, et les maj avant les min
//(on peut faire un bubble sort)

//si je fais export kiwi="fraise" donne le meme resultat que kiwi="fraise"

//si je fais export kiwi="fraise" puis je fais export kiwi     comme kiwi existe deja et que j'ai pas attribue de
//nouvelle cle, alors il se passe rien

//si je fais export adri (sans cle) et que je fais export alors ca apparait sans cle. Mais apres si je fais env
//je vois pas apparaitre adrien


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_env
{
    char         *name;
    char         *key;
    struct s_env *next;
}   t_env;

// Fonction pour créer un nouveau nœud
t_env *ft_envnode(const char *name, const char *key)
{
    t_env *new_node = (t_env *)malloc(sizeof(t_env));
    if (!new_node) {
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    new_node->name = strdup(name); // Copie du nom
    if (key) {
        new_node->key = strdup(key); // Copie de la valeur si elle existe
    } else {
        new_node->key = NULL;
    }
    new_node->next = NULL; // Pas de prochain nœud initialement
    return new_node;
}

// Fonction pour ajouter un nœud à la fin de la liste
t_env *ft_addenvnode(t_env *head, const char *name, const char *key) {
    if (!head) {
        return ft_envnode(name, key);
    }
    t_env *current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = ft_envnode(name, key);
    return head;
}

// Fonction pour afficher la liste chaînée
void print_list(const t_env *head) {
    const t_env *current = head;
    while (current) {
        printf("Nom : %s, Valeur : %s\n", current->name, current->key ? current->key : "NULL");
        current = current->next;
    }
}

// Fonction pour libérer la mémoire allouée à la liste chaînée
void free_list(t_env *head) {
    t_env *current = head;
    while (current) {
        t_env *temp = current;
        current = current->next;
        free(temp->name);
        free(temp->key);
        free(temp);
    }
}

//gcc -o ft_export sources/ft_export.c
//./ft_export
int main(int argc, char *argv[], char *env[]) {
    t_env *head = NULL; // Début de la liste chaînée

    // Gerer le cas ou env est NULL (voir bloc note)
    //Valable if (*env)
    for (int i = 0; env[i] != NULL; i++) {
        char *name = strtok(env[i], "=");
        char *key = strtok(NULL, "");
        head = ft_addenvnode(head, name, key);
    }

    // Afficher la liste chaînée
    print_list(head);

    // Libérer la mémoire
    free_list(head);

    return 0;
}
