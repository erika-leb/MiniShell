/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:53:57 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/22 15:32:57 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../gc/gc.h"

void print_redir(t_file **redir);

bool	ft_is_redir(char *s)
{
	if (ft_strcmp(s, "<") == 0 || ft_strcmp(s, "<<") == 0
		|| ft_strcmp(s, ">") == 0 || ft_strcmp(s, ">>") == 0)
		return (true);
	return (false);
}

t_file	*ft_lstnew_file(char *name, char token, t_gc *gc)
{
	t_file	*new;

	//perror("salut");
	new = gc_malloc(sizeof(t_cmd), gc);
	// if (new == NULL)
	// {
	// 	perror("malloc failed");
	// 	return (NULL); // ou exit a voir plus tard
	// }
	new -> name = name;
	new -> token = token;
	new -> next = NULL;
	//printf("new->name = %s token =%d\n", new->name, new->token );
	return (new);
}

int	ft_classification(char **tab, int i)
{
	//perror("ult");
	if (ft_strcmp(tab[i], "|") == 0)
		return (PIPE);
	if (i > 0 &&  ft_strcmp(tab[i - 1], "<") == 0)
		return (INPUT);
	if (i > 0 &&  ft_strcmp(tab[i - 1], "<<") == 0)
		return (HEREDOC);
	if (i > 0 &&  ft_strcmp(tab[i - 1], ">") == 0)
		return (TRUNC);
	if (i > 0 &&  ft_strcmp(tab[i - 1], ">>") == 0)
		return (APPEND);
	return (0); // pour linstqnt
}

void	ft_lstadd_back(t_file **lst, t_file *new)
{
	t_file	*current;

	if (!new)
		return ;
	if (!lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	//printf("ICI next = %p \n", new-> next);
	while (current -> next != NULL)
		current = current -> next;
	current -> next = new;
	//printf("LA next = %p \n", new-> next);
}

t_file	*create_redir(char **tab, int i, int last_i, t_gc *gc)
{
	t_file	*redir;
	t_file	*new;
	int		token;
//	t_file	*current;

	i++;
	// perror("ici");
	redir = NULL;
	//perror("ici");
	//printf("i = %d, last-i =%d\n", i, last_i);
	//printf("tab[i - 1] = %s\n", tab[i - 1]);
	while (last_i < i && tab[i - 1])
	{
	//	perror("la");
	//	printf("i -1 = %d\n", last_i - 1);
	//	printf("tab[i - 1] = %s\n", tab[last_i - 1]);
		// printf("tab = %s\n",tab[last_i - 1]);
		// printf("tab = %d\n",last_i);
		// printf("i -1 = %d\n", last_i - 1);
		if (last_i > 0 && ft_is_redir(tab[last_i - 1]) == true) // segfault ici a cause du - 1
		{
			//perror("c moyen cool");
			token = ft_classification(tab, last_i);
			//perror("c cool");
			new = ft_lstnew_file(tab[last_i], token, gc);
			//printf("new->name = %s token =%d, next = %p \n", new->name, new->token, new->next);
			// if (new == NULL)
			// 	exit(-1);
			if (!redir)
			{
				//perror("hola pri;ero");
				redir = new;
				//printf(" ICI next = %p \n", (*redir)->next);
			}
			else
				ft_lstadd_back(&redir, new);
		}
		//perror("cpa cool");
		last_i++;
	}
	//print_redir(redir);
	return (redir);
}

void	create_chain(char **tab, int i, int last_i, t_cmd **lst, t_gc *gc)
{
	t_cmd	*new;
	t_cmd	*current;

	//printf("i = %d, last_i = %d\n", i, last_i);
	new = gc_malloc(sizeof(t_cmd), gc);
	// if (new == NULL)
	// {
	// 	perror("malloc failed"); // clean tout et exit ici ou return pour exit apres
	// 	exit(-1);
	// }
	//new->cmd = create_cmd(tab, i, last_i);
	new->cmd = NULL;
	//perror("test6");
	new->redir = create_redir(tab, i, last_i, gc);
	//perror("test5");
	//print_redir(&(new->redir));
	new->fd_in = -1;
	new->fd_out = -1;
	new->next = NULL;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	//printf("current->next = %p\n", new->next);
	while (current -> next != NULL)
		current = current -> next;
	current -> next = new;
}

void	parsing(char **tab, t_cmd **lst, t_gc *gc)
{
	int	i;
	int	last_i;

	i = 0;
	last_i = 0;
	//perror("test12");
	while (tab[i])
	{
		//perror("test5");
		if (ft_strcmp(tab[i], "|") == 0)
		{
			//perror("test4");
			create_chain(tab, i, last_i, lst, gc);
			//perror("test5");
			last_i = i;
			//printf("i = %d, tab[i] = %s, last_i = %d\n", i, tab[i], last_i);
		}
		i++;
	}
	//perror("\nAPRES\n\n");
	//printf("i = %d, last_i = %d\n", i, last_i);
	//printf("tab[i] = %s, last_i = %d\n", tab[i], last_i);
	create_chain(tab, i - 1, last_i, lst, gc);
	//perror("test3");
}

void print_redir(t_file **redir)
{
    t_file *current;

    if (redir == NULL || *redir == NULL)
    {
        printf("  No redirections\n");
        return;
    }

    current = *redir; // Accéder à la liste chaînée
    while (current != NULL)
    {
        printf("  Redirection Name: %s, Token: %d\n", current->name, current->token);
        current = current->next;
    }
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

int main(void)
{
   t_cmd   *lst;
   int     i;

	t_gc		gc;
//	perror("test");

	gc_init(&gc);

   lst = NULL;
   char *arr[] = { "<<", "d", "echo", "okay", "<", "b", "baby", "<", "a", "<<", "c", "almost", ">", "e", "sure", "|", "cat", "|", "ls", "<", "K", "ls", ">>", "j", NULL};
	//char *arr[] = { "cat", "|", "ls", "<", "K", "ls", ">>", "j", NULL};
   ft_error_cases(arr, &gc);
	//perror("testi");
	parsing(arr, &lst, &gc);
	//perror("testf");
   t_cmd *current = lst;

   i = 0;
   while (current != NULL)
   {
       printf("Command:\n");
       for (int j = 0; current->cmd && current->cmd[j]; j++)
           printf("  cmd[%d]: %s\n", j, current->cmd[j]);

       printf("Redirections %d:\n", i);
       print_redir(&(current->redir));

       current = current->next; // Passe au nœud suivant
       i++;
       if (i > 10) // Limite de sécurité pour éviter une boucle infinie
           break;
   }
   gc_cleanup(&gc);
   return (0);
}

// il faudra parcourir la liste de redirections, note si au moins une est ko mais aller jusqu'au bout pour traiter les heredoc
// c'est toujours la derniere redicrection aui est pris en compte
// lorsau'on parcours la liste pour voir si tous les docs sont ouvrables, on arrete d'ouvrir a partir de la premiere erreur
// rajouter erreur si le tableau d'Adrien termine ou commence par | ou < > >> << ou si on a | apres < << > >> 'parse error,
// comprendre ce qu'il se passe quand on a une commande sans commande => commande ignoree (a quel point) on passe a la commade suivant ou on renvoie le prompt si pas d'autres commandes
