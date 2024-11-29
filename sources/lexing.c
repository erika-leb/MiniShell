/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:53:57 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/29 14:08:00 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../gc/gc.h"

// void print_redir(t_file **redir);

bool	ft_is_redir(char *s)
{
	if (ft_strcmp(s, "<") == 0 || ft_strcmp(s, "<<") == 0
		|| ft_strcmp(s, ">") == 0 || ft_strcmp(s, ">>") == 0)
		return (true);
	return (false);
}

bool	ft_is_str(char *s)
{
	if (ft_strcmp(s, "'|'") == 0)
		return (true);
	if (ft_strcmp(s, "'<'") == 0)
		return (true);
	if (ft_strcmp(s, "'>'") == 0)
		return (true);
	if (ft_strcmp(s, "'<<'") == 0)
		return (true);
	if (ft_strcmp(s, "'>>'") == 0)
		return (true);
	if (ft_strcmp(s, "'$?'") == 0)
		return (true);
	return (false);
}



// void	ft_fill_arr(char **arr, char **tab, int i, int last_i, t_gc *gc)
// {
// 	int	j;
// 	int	s;
// 	int	arr_s;
// 	int	k;

// 	arr_s = ft_arr_size(tab);

// 	j = last_i + 1;
// 	if (i == arr_s - 1)
// 		i++;
// 	k = 0;
// 	while (j < i)
// 	{
// 		if (ft_is_redir(tab[j]) == true)
// 			j++;
// 		else
// 		{
// 			//ft_put_words(arr, tab, j, k, gc);
// 			s = ft_strlen(tab[j]);
// 			if (ft_is_str(tab[j]) == true)
// 				arr[k] = ft_substr(tab[j], 1, s - 2, gc);
// 			else
// 			{
// 				arr[k] = gc_malloc(s + 1, gc);
// 				arr[k] = tab[j];
// 			}
// 			k++;
// 		}
// 		j++;
// 	}
// 	arr[k] = NULL;
// }

void	create_chain(char **tab, int i, int last_i, t_cmd **lst, t_gc *gc)
{
	t_cmd	*new;
	t_cmd	*current;

	new = gc_malloc(sizeof(t_cmd), gc);
	//perror("test");
	// if (new == NULL)
	// {
	// 	perror("malloc failed"); // clean tout et exit ici ou return pour exit apres
	// 	exit(-1);
	// }
	new->cmd = NULL;
	new->redir = create_redir(tab, i, last_i, gc);
		//perror("test2");
	new->cmd = cmd_arr(tab, i, last_i, gc);
	//new->here = NO_HERE;
		//perror("test3");
	// int	j = 0;
	// while(new->cmd[j])
	// {
	// 	printf("ici tab[%d] = %s\n", j, new->cmd[j]);
	// 	j++;
	// }
	new->fd_in = -2;
	new->fd_out = -2;
	new->code_status = 0;
	new->next = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current -> next != NULL)
		current = current -> next;
	current -> next = new;
}

// Fonction pour afficher le contenu d'une structure t_file
void print_redir(t_file *redir)
{
    t_file *current = redir;

    if (!current)
    {
        printf("  No redirections.\n");
        return;
    }

    printf("  Redirections:\n");
    while (current)
    {
        printf("    - Name: %s, Token: %d\n", current->name, current->token);
        current = current->next;
    }
}

// Fonction pour afficher le contenu d'une structure t_cmd
void print_cmd_list(t_cmd *cmd_list)
{
    t_cmd *current = cmd_list;
    int cmd_index = 0;

    while (current)
    {
        printf("Commande n°%d:\n", cmd_index);

        // Afficher la commande (cmd)
        if (current->cmd)
        {
            printf("  Command arguments:\n");
            for (int i = 0; current->cmd[i]; i++)
                printf("    cmd[%d]: %s\n", i, current->cmd[i]);
        }
        else
        {
            printf("  No command arguments.\n");
        }

        // Afficher les redirections
        print_redir(current->redir);

        // Afficher fd_in et fd_out
        printf("  fd_in: %d\n", current->fd_in);
        printf("  fd_out: %d\n", current->fd_out);

        // Passer au suivant
        current = current->next;
        cmd_index++;
    }
}

void	lexing(char **tab, t_cmd **lst, t_gc *gc) //ajouter les qutres elements
{
	int	i;
	int	last_i;

	i = 0;
	last_i = -1;
	while (tab[i])
	{
		if (ft_strcmp(tab[i], "|") == 0)
		{
			create_chain(tab, i, last_i, lst, gc);
			last_i = i;
		}
		i++;
	}
	//perror("test7");
	create_chain(tab, i - 1, last_i, lst, gc);

	// printf("\n AVANT OUVERTURE \n\n");
	// print_cmd_list(*lst);

	//perror("test");
	handle_redir(lst);

	printf("\n APRES OUVERTURE \n\n");
	print_cmd_list(*lst);
}

void	ft_error_cases(char **tab, t_gc *gc)
{
	int	s_arr;
	int	i;

	i = 1;
	s_arr = ft_arr_size(tab);
	if (ft_strcmp(tab[0], "|") == 0 || ft_strcmp(tab[s_arr -1], "|") == 0
		|| ft_strcmp(tab[s_arr -1 ], "<<") == 0
		|| ft_strcmp(tab[s_arr -1 ], ">>") == 0
		|| ft_strcmp(tab[s_arr -1 ], ">") == 0
		|| ft_strcmp(tab[s_arr -1 ], "<") == 0)
	{
		printf("syntax error near unexpected token\n");
		(gc_cleanup(gc), exit(EXIT_FAILURE));
	}
	while (tab[i])
	{
		if (ft_is_redir(tab[i - 1]) == true)
		{
			if (ft_is_redir(tab[i]) == true || ft_strcmp(tab[i], "|") == 0)
			{
				printf("syntax error near unexpected token\n");
				(gc_cleanup(gc), exit(EXIT_FAILURE));
			}
		}
		i++;
	}
}

// void print_redir(t_redir *redir) {
//     if (redir) {
//         printf("  File: %s\n", redir->file);
//         printf("  Type: %d\n", redir->type);
//     }
// }

// Fonction de test 1: Commande simple avec redirection ">"
// void test_case_1(t_gc *gc) {
//     t_cmd *lst = NULL;
//     char *arr[] = {"echo", "Hello", ">", "output.txt", NULL};
//     parsing(arr, &lst, gc);

//     // Affichage des résultats
//     t_cmd *current = lst;
//     while (current != NULL) {
//         printf("Command:\n");
//         for (int i = 0; current->cmd && current->cmd[i]; i++) {
//             printf("  cmd[%d]: %s\n", i, current->cmd[i]);
//         }
//         printf("Redirection:\n");
//         print_redir(&(current->redir));
//         current = current->next;
//     }
// }

// // Fonction de test 2: Commande avec redirection ">>"
// void test_case_2(t_gc *gc) {
//     t_cmd *lst = NULL;
//     char *arr[] = {"echo", "World", ">>", "append.txt", NULL};
//     parsing(arr, &lst, gc);

//     // Affichage des résultats
//     t_cmd *current = lst;
//     while (current != NULL) {
//         printf("Command:\n");
//         for (int i = 0; current->cmd && current->cmd[i]; i++) {
//             printf("  cmd[%d]: %s\n", i, current->cmd[i]);
//         }
//         printf("Redirection:\n");
//         print_redir(&(current->redir));
//         current = current->next;
//     }
// }

// // Fonction de test 3: Plusieurs commandes avec des pipes ("|")
// void test_case_3(t_gc *gc) {
//     t_cmd *lst = NULL;
//     char *arr[] = {"echo", "First", ">", "output.txt", "|", "cat", "|", "grep", "Hello", NULL};
//     parsing(arr, &lst, gc);

//     // Affichage des résultats
//     t_cmd *current = lst;
//     while (current != NULL) {
//         printf("Command:\n");
//         for (int i = 0; current->cmd && current->cmd[i]; i++) {
//             printf("  cmd[%d]: %s\n", i, current->cmd[i]);
//         }
//         printf("Redirection:\n");
//         print_redir(&(current->redir));
//         current = current->next;
//     }
// }

// // Fonction de test 4: Redirection multiple ">"
// void test_case_4(t_gc *gc) {
//     t_cmd *lst = NULL;
//     char *arr[] = {"echo", "Multiple", ">", "file1.txt", ">", "file2.txt", NULL};
//     parsing(arr, &lst, gc);

//     // Affichage des résultats
//     t_cmd *current = lst;
//     while (current != NULL) {
//         printf("Command:\n");
//         for (int i = 0; current->cmd && current->cmd[i]; i++) {
//             printf("  cmd[%d]: %s\n", i, current->cmd[i]);
//         }
//         printf("Redirection:\n");
//         print_redir(&(current->redir));
//         current = current->next;
//     }
// }

// // Fonction de test 5: Commande avec redirection "<"
// void test_case_5(t_gc *gc) {
//     t_cmd *lst = NULL;
//     char *arr[] = {"cat", "file.txt", "<", "input.txt", NULL};
//     parsing(arr, &lst, gc);

//     // Affichage des résultats
//     t_cmd *current = lst;
//     while (current != NULL) {
//         printf("Command:\n");
//         for (int i = 0; current->cmd && current->cmd[i]; i++) {
//             printf("  cmd[%d]: %s\n", i, current->cmd[i]);
//         }
//         printf("Redirection:\n");
//         print_redir(&(current->redir));
//         current = current->next;
//     }
// }

// // Fonction de test 6: Commande avec plusieurs redirections
// void test_case_6(t_gc *gc) {
//     t_cmd *lst = NULL;
//     char *arr[] = {"grep", "pattern", "<", "file1.txt", ">", "file2.txt", "<<", "here.txt", NULL};
//     parsing(arr, &lst, gc);

//     // Affichage des résultats
//     t_cmd *current = lst;
//     while (current != NULL) {
//         printf("Command:\n");
//         for (int i = 0; current->cmd && current->cmd[i]; i++) {
//             printf("  cmd[%d]: %s\n", i, current->cmd[i]);
//         }
//         printf("Redirection:\n");
//         print_redir(&(current->redir));
//         current = current->next;
//     }
// }

// // Fonction de test 7: Commande avec redirection complexe
// void test_case_7(t_gc *gc) {
//     t_cmd *lst = NULL;
//     char *arr[] = {"ls", "-l", "|", "grep", "txt", ">", "output.txt", NULL};
//     parsing(arr, &lst, gc);

//     // Affichage des résultats
//     t_cmd *current = lst;
//     while (current != NULL) {
//         printf("Command:\n");
//         for (int i = 0; current->cmd && current->cmd[i]; i++) {
//             printf("  cmd[%d]: %s\n", i, current->cmd[i]);
//         }
//         printf("Redirection:\n");
//         print_redir(&(current->redir));
//         current = current->next;
//     }
// }


// int main(void) {
//     t_gc gc;
//     gc_init(&gc);

//     // Appel des fonctions de test
//     test_case_1(&gc);
//     test_case_2(&gc);
//     test_case_3(&gc);
//     test_case_4(&gc);
//     test_case_5(&gc);
//     test_case_6(&gc);
//     test_case_7(&gc);

//     // Nettoyage de la mémoire allouée par le GC
//     gc_cleanup(&gc);

//     return 0;
// }



















// int main(void)
// {
//    t_cmd   *lst;
//    int     i;

// 	t_gc		gc;
// //	perror("test");

// 	gc_init(&gc);

//    lst = NULL;
//    //char *arr[] = { "<<", "d", "\"|\"", "echo", "okay", "<", "b", "baby", "<", "a", "<<", "c", "almost", ">", "e", "sure", "|", "cat", "|", "ls", "<", "K", "ls", ">>", "j", NULL};
// 	//char *arr[] = { "<", "|", "ls", "<", "K", "<<", "<" "j", "|", NULL};
//    ft_error_cases(arr, &gc);
// 	//perror("testi");
// 	parsing(arr, &lst, &gc);
// 	//perror("testf");
//    t_cmd *current = lst;

//    i = 0;
//    while (current != NULL)
//    {
//        printf("Command:\n");
//        for (int j = 0; current->cmd && current->cmd[j]; j++)
//            printf("  cmd[%d]: %s\n", j, current->cmd[j]);

//        printf("Redirections %d:\n", i);
//        print_redir(&(current->redir));

//        current = current->next; // Passe au nœud suivant
//        i++;
//        if (i > 10) // Limite de sécurité pour éviter une boucle infinie
//            break;
//    }
//    gc_cleanup(&gc);
//    return (0);
// }



// int main(void) {
//     t_cmd *lst = NULL;
//     int i;

//     t_gc gc;
//     gc_init(&gc);

//     // Cas 1: Commande simple avec redirection "<" et ">"
//     char *arr1[] = {
//         "echo", "Hello", ">", "output.txt", NULL
//     };
//     parsing(arr1, &lst, &gc);

//     // Cas 2: Commande avec redirection ">>"
//     char *arr2[] = {
//         "echo", "World", ">>", "append.txt", NULL
//     };
//     parsing(arr2, &lst, &gc);

//     // Cas 3: Plusieurs commandes avec redirections différentes
//     char *arr3[] = {
//         "echo", "First", ">", "output.txt", "|", "cat", "|", "grep", "Hello", NULL
//     };
//     parsing(arr3, &lst, &gc);

//     // Cas 4: Redirection multiple
//     char *arr4[] = {
//         "echo", "Multiple", ">", "file1.txt", ">", "file2.txt", NULL
//     };
//     parsing(arr4, &lst, &gc);

//     // Cas 5: Commande avec plusieurs arguments et redirection "<"
//     char *arr5[] = {
//         "cat", "file.txt", "<", "input.txt", NULL
//     };
//     parsing(arr5, &lst, &gc);

//     // Cas 6: Commande avec plusieurs redirections successives
//     char *arr6[] = {
//         "grep", "pattern", "<", "file1.txt", ">", "file2.txt", "<<", "here.txt", NULL
//     };
//     parsing(arr6, &lst, &gc);

//     // Cas 7: Commande avec redirection complexe
//     char *arr7[] = {
//         "ls", "-l", "|", "grep", "txt", ">", "output.txt", NULL
//     };
//     parsing(arr7, &lst, &gc);

//     // Cas 8: Commande avec redirection << (Heredoc)
//     char *arr8[] = {
//         "<<", "EOF", "echo", "here", "is", "heredoc", "EOF", NULL
//     };
//     parsing(arr8, &lst, &gc);

//     // Cas 9: Test d'une chaîne vide
//     char *arr9[] = {
//         NULL
//     };
//     parsing(arr9, &lst, &gc);

//     // Cas 10: Test avec plusieurs pipes sans redirection
//     char *arr10[] = {
//         "echo", "test", "|", "grep", "test", "|", "wc", NULL
//     };
//     parsing(arr10, &lst, &gc);

//     // Cas 11: Test avec des redirections multiples et des commandes complexes
//     char *arr11[] = {
//         "echo", "Hello", ">", "output1.txt", "|", "cat", "|", "grep", "Hello", ">", "output2.txt", NULL
//     };
//     parsing(arr11, &lst, &gc);

//     t_cmd *current = lst;
//     i = 0;

//     // Affichage des commandes et redirections
//     while (current != NULL) {
//         printf("Command %d:\n", i);
//         for (int j = 0; current->cmd && current->cmd[j]; j++) {
//             printf("  cmd[%d]: %s\n", j, current->cmd[j]);
//         }

//         if (current->redir) {
//             printf("Redirection %d:\n", i);
//             print_redir(&(current->redir));
//         }

//         current = current->next; // Passe au nœud suivant
//         i++;

//         // Limite de sécurité pour éviter une boucle infinie
//         if (i > 10) {
//             break;
//         }
//     }
// }


// void print_redir(t_file **redir)
// {
//     t_file *current;

//     if (redir == NULL || *redir == NULL)
//     {
//         printf("  No redirections\n");
//         return;
//     }

//     current = *redir; // Accéder à la liste chaînée
//     while (current != NULL)
//     {
//         printf("  Redirection Name: %s, Token: %d\n", current->name, current->token);
//         current = current->next;
//     }
// }


// il faudra parcourir la liste de redirections, note si au moins une est ko mais aller jusqu'au bout pour traiter les heredoc
// c'est toujours la derniere redicrection aui est pris en compte
// lorsau'on parcours la liste pour voir si tous les docs sont ouvrables, on arrete d'ouvrir a partir de la premiere erreur
// rajouter erreur si le tableau d'Adrien termine ou commence par | ou < > >> << ou si on a | apres < << > >> 'parse error,
// comprendre ce qu'il se passe quand on a une commande sans commande => commande ignoree (a quel point) on passe a la commade suivant ou on renvoie le prompt si pas d'autres commandes
