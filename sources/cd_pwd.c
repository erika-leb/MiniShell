/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:43:09 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/30 18:13:54 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static int	is_invalid_option(char *str)
{
	if (str[0] == '-' && str[1])
	{
		//perror("bill");
		return (TRUE);
	}
	//perror("boule");
	return (FALSE);
}

//On a pas pris PWD directement dans les var d'env car
void	ft_pwd(char **cmd, t_element *elements, t_gc *gc)
{
	char	*buff;
	char	*res;
	int		size;

	//perror("club");
	//dprintf(2, "%d\n", is_invalid_option(cmd[1]));
	if (cmd[1] && is_invalid_option(cmd[1]) == TRUE)
	{
		ft_buff_error("minishell: pwd: ", elements, gc);
		ft_buff_error(ft_substr(cmd[1], 0, 2, gc), elements, gc);
		ft_buff_error(": invalid option\n", elements, gc);
		ft_write_error(elements, gc);
		(gc_cleanup(gc), free_std(), exit(2));
	}
	//perror("buyer");
	buff = getcwd(NULL, 0);
	//perror("dallas");
	if (!buff)
	{
		//perror("doyle");
		ft_buff_error("pwd: error retrieving current directory", elements, gc);
		ft_buff_error(": getcwd: cannot access parent ", elements, gc);
		ft_buff_error("directories: No such file", elements, gc);
		ft_buff_error(" or directory\n", elements, gc);
		ft_write_error(elements, gc);
		//write(2, "pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 110);
		(gc_cleanup(gc), free_std(), exit(1));
		return ;
	}
	//perror("multipass");
	//ft_insert(buff, ft_strlen(buff), '\n');
	res = ft_strjoin_(buff, "\n", gc);
	size = ft_strlen(res);
	write(1, res, size);
	//printf("%s\n", buff); //pas de prbm dans enfant ?
	//write(1, buff, ft_strlen(buff));
	free(buff);
	(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
}

void	cd_error(t_built *built, t_gc *gc)
{
	//perror("die");
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
	//perror("hard");
}

static void	remove_old_env(t_env *head, t_built *built, t_gc *gc)
{
	// Etape 3 : je transforme head en tableau de chaine de caractere, je nettoie l'ancien env et j'en recree
	//un nouveau
	char	**new_env;

	new_env = ft_ltoa(head, gc);
	// gc_remove(gc, head);
	gc_remove(gc, built->elements->env);
	built->elements->env = new_env;
	gc_remove(gc, head);
}

// static void	change_var_name(t_env *current, char *buff, t_gc *gc)
// {
// 	char	*str;
// 	int		i;

// 	str = gc_malloc(ft_strlen(buff) + 1, gc);
// 	i = 0;
// 	while(buff[i])
// 	{
// 		str[i] = buff[i];
// 		i++;
// 	}
// 	str[i] = '\0';
// 	gc_remove(gc, current->key);
// 	current->key = buff;
// }

static void	change_var_name(t_env *current, const char *buff, t_gc *gc)
{
	char	*new_value;
	int		len;

	len = ft_strlen(buff);
	new_value = gc_malloc(len + 1, gc);
	ft_strncpy(new_value, buff, len);
	new_value[len] = '\0';
	gc_remove(gc, current->key);
	current->key = new_value;
}

static int	is_home_set(char **env, t_gc *gc)
{
	t_env	*head;
	t_env	*current;
	int		i;

	i = -1;
	head = NULL;
	while (env[++i])
		ft_adder(&head, env[i], gc);
	current = head;
	//change_env(buff, buff_old, built, gc);
	while (current)
	{
		if (ft_strcmp(current->name, "HOME") == 0)
		{
			if (ft_strcmp(current->key, "") == 0)
				return (3);
			return (TRUE);
		}
		current = current->next;
	}
	return (FALSE);
}

static int	cd_home(t_built *built, t_gc *gc)
{
	t_env	*head;
	t_env	*current;
	char	*str;
	int		i;

	i = -1;
	head = NULL;
	while (built->elements->env[++i])
		ft_adder(&head, built->elements->env[i], gc);
	current = head;
	//change_env(buff, buff_old, built, gc);
	while (current)
	{
		if (strcmp(current->name, "HOME") == 0)
		{
			str = current->key;
			if (chdir(str) != 0)
				return (1);
			else
				return (0);
		}
		current = current->next;
	}
	return (1);
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

	//perror("hans");
	if (built->cmd[1] && is_invalid_option(built->cmd[1]) == TRUE)
	{
		ft_buff_error("minishell: pwd: ", built->elements, gc);
		ft_buff_error(ft_substr(built->cmd[1], 0, 2, gc), built->elements, gc);
		ft_buff_error(": invalid option\n", built->elements, gc);
		ft_write_error(built->elements, gc);
		return ;
	}
	buff_old = getcwd(NULL, 0);
	if (!buff_old)
	{
		//perror("doyle");
		ft_buff_error("cd: error retrieving current directory", built->elements, gc);
		ft_buff_error(": getcwd: cannot access parent ", built->elements, gc);
		ft_buff_error("directories: No such file", built->elements, gc);
		ft_buff_error(" or directory\n", built->elements, gc);
		ft_write_error(built->elements, gc);
		//write(2, "pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n", 110);
		return ;
	}
	// if (!buff_old)
	// 	return ;
	//printf("OLDPWD = %s\n\n\n", buff_old);
	if (!built->cmd[1])
	{
		//perror("petit lapin");
		if (is_home_set(built->elements->env, gc) == TRUE)
		{
			//perror("fait des compotes");
			if (cd_home(built, gc) == 1) // ca s est mal passe
			{
				//perror("pour maman lapin");
				cd_error(built, gc);
				built->elements->exit_status = ft_itoa(1, gc);
				free(buff_old);
				return ; //exit_status
			}
			free(buff_old);
			return ; //exit_status
		}
		else if (is_home_set(built->elements->env, gc) == FALSE)
		{
			write(2, "minishell: cd : HOME not set\n", 30);
			free(buff_old);
			return ; //exit_status
		}
		else
		{
			free(buff_old);
			return ; //exit_status
		}
	}
	if (access(built->cmd[1], F_OK) == 0 && built->cmd[2])
	{
		//perror("prehistorique");
		write(2, "minishell: cd: too many arguments\n", 35);
		free(buff_old);
		return ; //exit_status
	}
	if (chdir(built->cmd[1]) != 0)
	{
		//perror("anyanka");
		cd_error(built, gc);
		built->elements->exit_status = ft_itoa(1, gc);
		free(buff_old);
		return ; //exit_status
	//
	// 	perror("test");
	 	//printf("errno = %d\n", errno); //mettre le message d erreur en fonction de errno ici
	//
	}
	buff = getcwd(NULL, 0);
	if (!buff)
	{
		free(buff_old);
		return ; //exit_status
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
		}
		if (strcmp(current->name, "PWD") == 0)
		{
			change_var_name(current, buff, gc);
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
