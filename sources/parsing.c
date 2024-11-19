/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 18:56:41 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/18 17:31:34 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_token_classification(char **tab, int i, t_token **lst, int cmd)
{
	(void) lst;
	
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
	if (cmd == 1)
		return (ARG);
	if (cmd == 0)
		return (CMD);
	return (0); // pour linstqnt	 
}

//pour output ; 		
// if (current->token == TRUNC || current->token == APPEND)
//			printf("name = %s\n", current->name);

void	ft_move_inputs(t_token **lst, int *i, int *nb_in)
{
	t_token *current;
	t_token	*last;
	int		k;

	last = *lst;
	current = last->next;
	if ((last->token == INPUT || last->token == HEREDOC) && (*nb_in) == 0)
		(*nb_in) = 1;
	k = 1;
	while (k < (*i))
	{
		current = current->next;
		last = last->next;
		k++;
	}
//	printf("test\n");
	while (current != NULL)
	{
		(*i)++;
		if (current->token == INPUT || current->token == HEREDOC)
		{
			last->next = current->next;
			current->next = *lst;
			*lst = current;
			(*nb_in)++;
			return ;
		}
		current = current->next;
		last = last->next;
	}
}
void	ft_move_outputs(t_token **lst, int *i, int *nb_out)
{
	t_token *current;
	t_token	*last;
	t_token	*ult;
	int		k;

	last = *lst;
	current = last->next;
	// if ((last->token == APPEND || last->token == TRUNC) && (*nb_in) == 0) // a modifier
	// 	(*nb_in) = 1;
	k = 0;
	while (k < (*i))
	{
		current = current->next;
		last = last->next;
		k++;
	}
	printf("test\n");
	while (current != NULL)
	{
		(*i)++;
		if (current->token == APPEND || current->token == TRUNC)
		{
			last->next = current->next;
			current->next = NULL;
			ult = ft_lstlast(*lst);
			ult->next = current;
			(*nb_out)++;
			return ;
		}
		printf("test2\n");
		current = current->next;
		last = last->next;
	}
}

void	ft_move_chains(t_token **lst)
{
	int		i;
	int 	nb_in;
	int		size_lst;
	int		nb_out;

	i = 0;
	size_lst = ft_lstsize(lst);
	nb_in = 0;
	nb_out = 0;
	while (i < size_lst - 1)
		ft_move_inputs(lst, &i, &nb_in);
	i = 0;
	while (i < size_lst - 1)
		ft_move_outputs(lst, &i, &nb_out);
	printf("in = %d\n", nb_in);
}

t_token	*parsing(char **tab, t_token *lst)
{
	t_token	*new;
	int		token;
	int		i;
	int		cmd;

	lst = NULL;
	i = 0;
	cmd = 0;
	while (tab[i] != NULL)
	{
		if (ft_strcmp(tab[i], "<") != 0 && ft_strcmp(tab[i], "<<") != 0 && ft_strcmp(tab[i], ">") != 0 && ft_strcmp(tab[i], ">>") != 0)
		{
			token = ft_token_classification(tab, i, &lst, cmd);
			if (token == CMD)
				cmd = 1;
			if (token == PIPE)
				cmd = 0;
			new = ft_lstnew(tab[i], token);
			if (new == NULL)
			{
				//clean (history, line, reste de la chaine, travail adien)
				exit(1);
			}
			if (!lst)
			{
				lst = new;
			}
			else
				ft_lstadd_back(&lst, new);
		}
		i++;
		//printf("i = %d, tab[i] = %s\n", i, tab[i]);
	}
	ft_move_chains(&lst);
	new = ft_lstlast(lst);
	if (new->token == PIPE)
	{
		// liberer tout
		printf("Error : no ending with pipe");
		exit(0);
	}
	return (lst);
}

int	main(void)
{
	t_token	*lst;
	int		i;

	lst = NULL;
	char *arr[] = {"<<", "d", "echo", "okay", "<", "b", "baby", "<", "a", "<<", "c", "almost", ">", "e", "sure", "|", "cat", "|", "ls", "ls", ">>", "j", NULL};
	lst = parsing(arr, lst);
	
	
	t_token *current = lst;
    
	i=0;
    while (current != NULL)
    {
        printf("Name: %s, Token: %d\n", current->name, current->token);
        current = current->next;  // Avance au nœud suivant
		i++;
		if (i > 10)
			break;
    }
	return (0);
}
