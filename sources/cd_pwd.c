/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:43:09 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/23 18:26:13 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

//On a pas pris PWD directement dans les var d'env car
void	ft_pwd(t_element *elements, t_gc *gc)
{
	static char	buff[70000];

	(void) gc;
	(void) elements;
	getcwd(buff, sizeof(buff));
	ft_insert(buff, ft_strlen(buff), '\n');
	write(1, buff, ft_strlen(buff));
}

//hedge case : si je suis dans un Dossier que je supprime, alors en faisant cd .. j'ai un message d'erreur
//Demander a Paul. Regarder la conversation avec adri sur Discord
void	ft_cd(t_built *built, t_gc *gc)
{
	static char	buff_old[70000];
	static char	buff[70000];
	int			i;
	t_env		*head;
	t_env		*current;

	getcwd(buff_old, sizeof(buff_old));
	printf("OLDPWD = %s\n\n\n", buff_old);
	if (chdir(built->cmd[1]) != 0)
	{
		printf("errno = %d\n", errno); //mettre le message d erreur en fonction de errno ici
	}
	getcwd(buff, sizeof(buff));
	printf("PWD = %s\n\n\n", buff);
	//Si OLDPWD a ete unset on fait rien le concernant. Idem pour PWD

	//Etape 1 : On transforme le tableau de chaines de caracteres (built->elements->env)
	//en liste chainee (head) pour reperer facilement ou se situe PWD et OLDPWD
	i = -1;
	head = NULL;
	while (built->elements->env[++i])
			ft_adder(&head, built->elements->env[i], gc);
	///////////////////////////////////////////////////////////////////////////

	//Etape 2 : On parcourt head et on recherche OLDPWD. S'il existe on change sa key (valeur).
	current = head;
	while (current)
	{
		if (strcmp(current->name, "OLDPWD") == 0)
		{
			//Si je tombe sur OLDPWD je lui attribue la valeur buff_old.
			//Pour se faire je malloc une chaine de la meme taille que buff_old et ce sera la nouvelle cle.
			gc_remove(gc, current->key);
			current->key = buff_old;

		}
		if (strcmp(current->name, "PWD") == 0)
		{
			//Si je tombe sur OLDPWD je lui attribue la valeur buff.
			//Pour se faire je malloc une chaine de la meme taille que buff et ce sera la nouvelle cle.
			gc_remove(gc, current->key);
			current->key = buff;

		}
		current = current->next;
	}
	/////////////////////////////////////////////////////////////////

	// Etape 3 : je transforme head en tableau de chaine de caractere, je nettoie l'ancien env et j'en recree
	//un nouveau
	char	**new_env;

	new_env = ft_ltoa(head, gc);
	gc_remove(gc, head);
	gc_remove(gc, built->elements->env);
	built->elements->env = new_env;

	// int s_arr;

	// s_arr = ft_arr_size(built->elements->env);
	// printf("s = %d\n", s_arr);
	// i = 0;
	// while( i <= s_arr)
	// {
	// 	printf("element->env %i = %s\n", i, built->elements->env[i]);
	// 	//printf("myenv %i = %s\n", i, elements->env[i]);
	// 	i++;
	// }




}
