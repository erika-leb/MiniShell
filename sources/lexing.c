/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 15:53:57 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/27 18:08:46 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "../gc/gc.h"

// void print_redir(t_file **redir);

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

static void	ft_init_values(int *i, int *p_max, t_element *elements)
{
	(*i) = 0;
	elements->child_to_wait = 0;
	(*p_max) = 0;
}

void	hedge_case_1(char **tab, t_element *elements, t_gc *gc)
{
	int	i;
	int	p_max;

	//perror("lop");
	ft_init_values(&i, &p_max, elements);
	//printf("p_max = %d\n", p_max);
	//printf("p_max[0] = %s tchoupi\n", tab[p_max]);
	while (1)
	{
		if (ft_strcmp(tab[p_max], "|") == 0
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
	while (i < p_max - 1)
	{
		if (ft_strcmp(tab[i], "<<") == 0)
			ft_open_heredoc_error(tab[i + 1], elements, gc);
		i++;
	}
}

void	lexing(char **tab, t_cmd **lst, t_element *elements, t_gc *gc)
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
	{
		//perror("titi");
		hedge_case_1(tab, elements, gc);
	}
	else
	{
		//perror("grosminet");
		handle_redir(lst, elements, gc);
	}

	// printf("\n APRES OUVERTURE \n\n");
	// print_cmd_list(*lst);
	//perror("normalement les messages ne sont pas encore affiches");
	//printf("error = %s\n", elements->error);
	ft_write_error(elements, gc);
	//printf("cmd = %d\n", elements->lst->cmd);
	//printf("cmd[0] = %s\n", elements->lst->cmd[0]);
}

// il faudra parcourir la liste de redirections, note si au moins une est ko mais aller jusqu'au bout pour traiter les heredoc
// c'est toujours la derniere redicrection aui est pris en compte
// lorsau'on parcours la liste pour voir si tous les docs sont ouvrables, on arrete d'ouvrir a partir de la premiere erreur
// rajouter erreur si le tableau d'Adrien termine ou commence par | ou < > >> << ou si on a | apres < << > >> 'parse error,
// comprendre ce qu'il se passe quand on a une commande sans commande => commande ignoree (a quel point) on passe a la commade suivant ou on renvoie le prompt si pas d'autres commandes
