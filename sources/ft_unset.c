/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/22 12:02:05 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

//Demander quels sont les pieges a eviter

//NB : Pour unset il faut en fait s'assurer que si je supprime toutes les var d'env alors ca remet OLDPWD, PWD et SHLVL? A priori nope,
//on a pas a gerer ca vu que env -i ne peut se faire que hors du minishell.

//EN REVANCHE, au debut de minishell je dois verifier que si env est vide (ou NULL?) alors je vais chercher PWD, je simule (?)  SHLVL et
//j'incorpore OLPWD sans cle

//NB : pour unset on pourra facilement supprimer 1 ou plsr var en utilisant la meme logique que export de la chaine de caractere :
//je créé une liste temporaire, je supprime le(s) noeud(s) souhaité(s) et je recréé un nouveau tableau envv mis a jour.
//En oubliant pas que si je supprime tout il doit quand meme y avoir SHLVL OLDPWD et PWD.

//tranformer env en liste chainee. remove le noeud name (si y'en a pas on fait rien). je retransforme en envv et je remplace l'ancien env.

static int     ft_initunset(t_env **ptr_head, char **env, char **argv, t_gc *gc)
{
	int i;

	if (!argv[1])
		return (1);
	*ptr_head = NULL;
	i = -1;
	while (env[++i])
		ft_adder(ptr_head, env[i], gc);
	return (0);
}

static void ft_freeun(t_env **current, t_env **previous, t_env **head, t_gc *gc)
{
	t_env *temp;

	temp = *current;
	if (*previous)
		(*previous)->next = (*current)->next;
	else
		*head = (*current)->next;
	*current = (*current)->next;
	gc_remove(gc, temp->name);
	gc_remove(gc, temp->key);
	gc_remove(gc, temp);
}

void ft_unset(t_element *element, char **argv, t_gc *gc)
{
	t_env   *head;
	t_env   *current;
	t_env   *previous;
	int     i;
	char	**adder;

	if (ft_initunset(&head, element->env, argv, gc))
		return ;
	i = 1; // On commence à cmd[1]
	while (argv[i])
	{
		current = head;
		previous = NULL;
		while (current)
		{
			if (strcmp(current->name, argv[i]) == 0)
				ft_freeun(&current, &previous, &head, gc);
			else
			{
				previous = current;
				current = current->next;
			}
		}
		i++;
	}
	//a la fin on modifie env et on free l'ancienne version !!!!!!!
	//ft_unset peut retourner un char **array comme ca dans ft_builtin on fera juste env = ft_unset
	// ft_printexport(head);


	adder = ft_ltoa(head, gc);
	gc_remove(gc, head);
	gc_remove(gc, element->env);
	element->env = adder;

	// int s_arr;
	// s_arr = ft_arr_size(element->env);
	// printf("s = %d\n", s_arr);
	// i = 0;
	// while( i <= s_arr)
	// {
	// 	printf("element1->env %i = %s\n", i, element->env[i]);
	// 	//printf("myenv %i = %s\n", i, elements->env[i]);
	// 	i++;
	// }
}
