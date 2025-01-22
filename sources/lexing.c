/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:53:57 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/22 15:22:36 by ele-borg         ###   ########.fr       */
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
	if (ft_strcmp(s, "\n|") == 0)
		return (true);
	if (ft_strcmp(s, "\n<") == 0)
		return (true);
	if (ft_strcmp(s, "\n>") == 0)
		return (true);
	if (ft_strcmp(s, "\n<<") == 0)
		return (true);
	if (ft_strcmp(s, "\n>>") == 0)
		return (true);
	return (false);
}

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
        dprintf(2,"  No redirections.\n");
        return;
    }

    dprintf(2,"  Redirections:\n");
    while (current)
    {
        dprintf(2,"    - Name: %s, Token: %d\n", current->name, current->token);
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
        dprintf(2, "Commande n°%d:\n", cmd_index);

        // Afficher la commande (cmd)
        if (current->cmd)
        {
            dprintf(2, "  Command arguments:\n");
            for (int i = 0; current->cmd[i]; i++)
                dprintf(2,"    cmd[%d]: %s\n", i, current->cmd[i]);
        }
        else
        {
            dprintf(2, "  No command arguments.\n");
        }

        // Afficher les redirections
        print_redir(current->redir);

        // Afficher fd_in et fd_out
        dprintf(2,"  fd_in: %d\n", current->fd_in);
        dprintf(2,"  fd_out: %d\n", current->fd_out);

        // Passer au suivant
        current = current->next;
        cmd_index++;
    }
}

int	ft_istok_2(char *av2)
{
	if (!ft_strcmp(av2, "<") || !ft_strcmp(av2, ">")
		|| !ft_strcmp(av2, "<<") || !ft_strcmp(av2, ">>"))
		return (1);
	return (0);
}

void	hedge_case_1(char **tab, t_element *elements)
{
	int	i;
	int	p_max;

	i = 0;
	elements->child_to_wait = 0;
	p_max = 0;
	while (1)
	{
		if (!ft_strcmp(tab[p_max], "|")
			&& tab[p_max + 1] && !ft_strcmp(tab[p_max + 1], "|"))
			break ;
		if (tab[p_max] && ft_istok_(tab[p_max]) && tab[p_max + 1]
			&& ft_istok_(tab[p_max + 1]))
			break ;
		if (tab[p_max] && ft_istok_(tab[p_max]) && tab[p_max + 1]
			&& !ft_strcmp(tab[p_max + 1], "|"))
			break ;
		if (ft_istok_(tab[p_max]) && !tab[p_max + 1])
			break;
		p_max++;
	}
	while (i < p_max - 1) //stric ou pas ?
	{
		if (ft_strcmp(tab[i], "<<") == 0)
			ft_open_heredoc_error(tab[i + 1]);
		i++;
	}
}

void	lexing(char **tab, t_cmd **lst, t_element *elements, t_gc *gc) //ajouter les qutres elements
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
	if (tab && tab[0] && ft_strcmp(tab[0], "\n") == 0)
		hedge_case_1(tab, elements);
	else
		handle_redir(lst, elements, gc);

	// printf("\n APRES OUVERTURE \n\n");
	// print_cmd_list(*lst);
	ft_write_error(elements, gc);
	//printf("cmd = %d\n", elements->lst->cmd);
	//printf("cmd[0] = %s\n", elements->lst->cmd[0]);
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


// il faudra parcourir la liste de redirections, note si au moins une est ko mais aller jusqu'au bout pour traiter les heredoc
// c'est toujours la derniere redicrection aui est pris en compte
// lorsau'on parcours la liste pour voir si tous les docs sont ouvrables, on arrete d'ouvrir a partir de la premiere erreur
// rajouter erreur si le tableau d'Adrien termine ou commence par | ou < > >> << ou si on a | apres < << > >> 'parse error,
// comprendre ce qu'il se passe quand on a une commande sans commande => commande ignoree (a quel point) on passe a la commade suivant ou on renvoie le prompt si pas d'autres commandes
