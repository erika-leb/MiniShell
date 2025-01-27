/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:43:09 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/26 19:10:14 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

//On a pas pris PWD directement dans les var d'env car
void	ft_pwd(t_element *elements, t_gc *gc)
{
	char	*buff;

	(void) gc;
	(void) elements;
	buff = getcwd(NULL, 0);
	if (!buff)
		return ;
	//ft_insert(buff, ft_strlen(buff), '\n');
	printf("%s\n", buff);
	//write(1, buff, ft_strlen(buff));
	free(buff);
	(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
}

void	cd_error(t_built *built, t_gc *gc)
{
	ft_buff_error("minishell: cd: ", built->elements, gc);
	ft_buff_error(built->cmd[1], built->elements, gc);
	// if (errno == 13)
	// 	ft_buff_error(": Permission denied\n", built->elements, gc);
	// if (errno == 14)
	// 	ft_buff_error(": Bad address\n", built->elements, gc);
	// if (errno == 22)
	// 	ft_buff_error(": Invalid argument\n", built->elements, gc);
	// if (errno == 2)
	// 	ft_buff_error(": No such file or directory\n", built->elements, gc);
	// if (errno == 34)
	//	ft_buff_error(": Result too large\n", built->elements, gc);
	ft_buff_error(": ", built->elements, gc);
	ft_buff_error(strerror(errno), built->elements, gc);
	ft_buff_error("\n", built->elements, gc);
	ft_write_error(built->elements, gc);
}

static void	remove_old_env(t_env *head, t_built *built, t_gc *gc)
{
	// Etape 3 : je transforme head en tableau de chaine de caractere, je nettoie l'ancien env et j'en recree
	//un nouveau
	char	**new_env;

	new_env = ft_ltoa(head, gc);
	gc_remove(gc, head);
	gc_remove(gc, built->elements->env);
	built->elements->env = new_env;
}

static void	change_var_name(t_env *current, char *buff, t_gc *gc)
{
	char	*str;
	int		i;

	str = gc_malloc(ft_strlen(buff) + 1, gc);
	i = 0;
	while(buff[i])
	{
		str[i] = buff[i];
		i++;
	}
	str[i] = '\0';
	gc_remove(gc, current->key);
	current->key = buff;
}

//hedge case : si je suis dans un Dossier que je supprime, alors en faisant cd .. j'ai un message d'erreur
//Demander a Paul. Regarder la conversation avec adri sur Discord // en faisant cd .. verifier que le directory existent avec access ?
void	ft_cd(t_built *built, t_gc *gc)
{
	char	*buff_old;
	char	*buff;
	int		i;
	t_env	*head;
	t_env	*current;

	buff_old = getcwd(NULL, 0);
	if (!buff_old)
		return ;
	//printf("OLDPWD = %s\n\n\n", buff_old);
	if (chdir(built->cmd[1]) != 0)
	{
		cd_error(built, gc);
		built->elements->exit_status = ft_itoa(1, gc);
		return ;
	//
	// 	perror("test");
	 	//printf("errno = %d\n", errno); //mettre le message d erreur en fonction de errno ici
	//
	}
	buff = getcwd(NULL, 0);
	if (!buff)
	{
		free(buff_old);
		return ;
	}
	//printf("PWD = %s\n\n\n", buff);
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
	//change_env(buff, buff_old, built, gc);
	while (current)
	{
		if (strcmp(current->name, "OLDPWD") == 0)
		{
			change_var_name(current, buff_old, gc);
			//Si je tombe sur OLDPWD je lui attribue la valeur buff_old.
			//Pour se faire je malloc une chaine de la meme taille que buff_old et ce sera la nouvelle cle.
			// gc_remove(gc, current->key);
			// current->key = buff_old;

		}
		if (strcmp(current->name, "PWD") == 0)
		{
			change_var_name(current, buff, gc);
			//Si je tombe sur OLDPWD je lui attribue la valeur buff.
			//Pour se faire je malloc une chaine de la meme taille que buff et ce sera la nouvelle cle.
			// gc_remove(gc, current->key);
			// current->key = buff;

		}
		current = current->next;
	}
	free(buff);
	free(buff_old);
	/////////////////////////////////////////////////////////////////
	remove_old_env(head, built, gc);
	built->elements->exit_status = ft_itoa(0, gc);
	// Etape 3 : je transforme head en tableau de chaine de caractere, je nettoie l'ancien env et j'en recree
	//un nouveau
	// char	**new_env;

	// new_env = ft_ltoa(head, gc);
	// gc_remove(gc, head);
	// gc_remove(gc, built->elements->env);
	// built->elements->env = new_env;

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
