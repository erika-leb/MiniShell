/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/02/06 13:16:07 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

//static
void ft_printexport(const t_env *head, t_element *elements, t_gc *gc)
{
    const t_env *current = head;
	perror("medellin");
    //Tout mettre dans un buffer, comme pour l'erreur de ft_exit
    while (current)
	{
		perror("pegate");
        if (current->key)
		{
			perror("en la casa");
			ft_buff_error("export ", elements, gc);
			ft_buff_error(current->name, elements, gc);
			ft_buff_error("=\"", elements, gc);
			ft_buff_error(current->key, elements, gc);
			ft_buff_error("\"\n", elements, gc);
			perror("jeanne");
			ft_write_mess(elements, gc);
			perror("marie");
		}
            //printf("export %s=\"%s\"\n", current->name, current->key);
        else
		{
			ft_buff_error("export ", elements, gc);
			ft_buff_error(current->name, elements, gc);
			ft_buff_error("\n", elements, gc);
			ft_write_mess(elements, gc);
		}
            //printf("export %s\n", current->name); //enlever le printf
        perror("salvaje");
		current = current->next;
    }
	perror("bellaco");
}
//static
// static void ft_freelexport(t_env *head)
// {
// 	t_env *current = head;
// 	while (current)
// 	{
// 		t_env *temp = current;
// 		current = current->next;
// 		free(temp->name);
// 		free(temp->key);
// 		free(temp);
// 	}
// }

//NB : quand on aura fini export il faudra reecrire ft_getenvv car a partir de mtn
//on travaille plus avec l'environnement bash mais avec notre tableau envv ??
//static
void ft_adder(t_env **head, char *str, t_gc *gc)
{
	char  **adder;

	adder = gc_calloc(2 + 1, sizeof(char *), gc);//gc_cleaner
	adder[0] = ft_cut(str, '=', 0, gc);//dans l'exportation je concatene/expand deja bien.
	adder[1] = ft_cut(str, '=', 1, gc);
	*head = ft_addenvnode(*head, adder[0], adder[1], gc);
	gc_remove(gc, adder[0]);//useless
	gc_remove(gc, adder[1]);//useless
	gc_remove(gc, adder);//useless aussi I guess
}

//J'ai aussi remarque que si le user ecrit bonjour="cava \"oui\" et toi"  alors ca donne bonjour="cava \"oui\" et toi"
//                                                                            au lieu de bonjour="cava \oui\ et toi"
//Je pense que bash concatene deja les argv donc pour voir comment se comporte reellement ft_export dans minishell il faut
//le tester grandeur nature.
//A l'affichage (quand je fais ft_export(env, NULL)) il faudra juste ajouter des \ devant les $ et les " (ft_write)

//a chaque fois que je fais cd je peux directement fair appel a ft_export
//pour modifier OLPWD et PWD
char **ft_export(t_element *element, char **argv, t_gc *gc, int ch) //si ch == 1, exit, sinon exit_status a changer
{
	// Gerer le cas ou env est NULL : env -i ./minishell (voir bloc note)
	t_env *head;
	char  **adder;
	int   i;
	int	  flag;
	int	code;

	head = NULL;
	i = -1;
	perror("freaky");
	while (element->env[++i])
		ft_adder(&head, element->env[i], gc);
	perror("no hace falta");
	if (!argv[1])
	{
		perror("bailamos");
		(ft_bbsort(head), ft_printexport(head, element, gc));
		perror("bebecita");
		gc_remove(gc, head);
		perror("bebezota");
		dprintf(2, "ch =%d\n", ch);
		if (ch == 0)
		{
			perror("nasty");
			element->exit_status = ft_itoa(0, gc);
			return (NULL);//
		}
		else
		{
			perror("kinky");
			(gc_cleanup(gc), free_std(), exit(0));
		}
	}
	perror("toma");
	if (argv[1][0] == '-')
	{
		write(2, "minishell: export: No option(s) allowed\n", 41);
		gc_remove(gc, head);
		// element->exit_status = ft_itoa(2, gc);
		// return (NULL);
		if (ch == 0)
		{
			//perror("nasty");
			element->exit_status = ft_itoa(2, gc);
			return (NULL);//
		}
		else
		{
			//perror("kinky");
			(gc_cleanup(gc), free_std(), exit(2));
		}
	}
	//perror("makinon");
	i = 0;
	flag = 0;
	//flag = ft_filler(argv, element, gc, head);//reprend le while en dessous
	while (argv[++i])
	{
		if (!ft_exparser(argv[i], element, gc))
			ft_adder(&head, argv[i], gc);
		else
		{
			// perror("test");
			element->exit_status = ft_itoa(1, gc);
			code = 1;
			flag = 1;
		}
	}
	//perror("city");
	adder = ft_ltoa(head, gc);
	gc_remove(gc, head);
	//Retirer la variable _ dans la structure env de minishell
	//(ca n'apparait pas dans bash --posix) ??
	//Sur ?? Car ca apparait dans bash --posix

	//Pas sur que ca remplace correctement env
	//(checker en modifiant &env)

	gc_remove(gc, element->env);
	element->env = adder;
	// s_arr = ft_arr_size(element->env);
	// printf("s = %d\n", s_arr);
	// i = 0;
	// while( i <= s_arr)
	// {
	// 	printf("element->env %i = %s\n", i, element->env[i]);
	// 	//printf("myenv %i = %s\n", i, elements->env[i]);
	// 	i++;
	// }
	//perror("prender");
	if (!flag)
	{
		element->exit_status = ft_itoa(0, gc);
		code = 0;
	}
	// printf("exit status : %s\n", element->exit_status);


	if (ch == 0)
	{
		//perror("reggeaton");
		return (adder);//
	}
	else
	{
		//perror("dembow");
		(gc_cleanup(gc), free_std(), exit(code));
	}

	//return (adder);//a tej plus tard
}
