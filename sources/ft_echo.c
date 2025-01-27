
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 18:19:30 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/20 14:30:34 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//mettre a la norme et demander a adri ce qu'il a fait pour $?

#include "../minishell.h"
#include "../gc/gc.h"

static int ft_is_option(char *opt)
{
	int i;

	i = 1;
	if (opt[0] != '-')
		return (FALSE);
	while (opt[i])
	{
		if (opt[i] != 'n')
			return (FALSE);
		i++;
	}
	return (TRUE);
}
static void	ft_fill_buf(int *i, int *k, char **cmd, char *buf)
{
	int j;

	j = 0;
	while (cmd[*i][j])
		buf[(*k)++] = cmd[*i][j++];
	(*i)++;
}

// static void	ft_init_var(int *i, int *k, int *n, int *f)
// {
// 	(*i) = 1;
// 	(*k) = 0;
// 	(*n) = 0;
// 	(*f) = 0;
// }

static void	write_error(int f, t_gc *gc)
{
	if (f == -1) // a veririfer a la fin
	{
		perror("minishell: echo: write error");
		(gc_cleanup(gc), free_std(), exit(1));
	}
}

static void	no_arg(int *n, char **cmd, t_gc *gc)
{
	if (!cmd[1])
	{
		write(1, "\n", 1);
		(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
	}
	if (ft_is_option(cmd[1]) == TRUE)
		(*n) = 1;
}

void	ft_echo(char **cmd, t_gc *gc)
{
	int i;
	int k;
	int n;
	int	f;
	char buf[70000];

	//ft_init_var(&i, &k, &n, &f);
	i = 1;
	k = 0;
	n = 0;
	f = 0;
	no_arg(&n, cmd, gc);
	// if (!cmd[1])
	// {
	// 	write(1, "\n", 1);
	// 	(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
	// }
	// if (ft_is_option(cmd[i]) == TRUE)
	// 	n = 1;
	while (cmd[i] && ft_is_option(cmd[i]) == TRUE)
		i++;
	//Expand les var d'env avec la fonction d'adri
	while (cmd[i])
	{
		if (f == 1)
			buf[k++] = ' ';
		ft_fill_buf(&i, &k, cmd, buf);
		f = 1;
	}
	if (n == 0)
		buf[k++] = '\n';
	f = write(1, buf, k); //a verifier le 1
	write_error(f, gc);
	(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
}

// int main() {
// 	// Test 1 : Avec l'option -n
// 	char *cmd1[] = {"echo", "-n", "Hello", "World!", NULL};
// 	ft_echo(cmd1); // "Hello World!" sans saut de ligne

// 	// Test 2 : Sans l'option -n
// 	char *cmd2[] = {"echo", "Hello", "there", "friend!", NULL};
// 	ft_echo(cmd2); // "Hello there friend!" avec saut de ligne

// 	// Test 3 : Seulement l'option -n
// 	char *cmd3[] = {"echo", "-n", NULL};
// 	ft_echo(cmd3); // Aucun message, pas de saut de ligne

// 	// Test 4 : Plusieurs options -n
// 	char *cmd4[] = {"echo", "-n", "-n", "Hello", "again", NULL};
// 	ft_echo(cmd4); // "Hello again" sans saut de ligne

// 	// Test 5 : Aucun argument (uniquement echo)
// 	char *cmd5[] = {"echo", "-nnnnnnnnj", "ca", NULL};
// 	ft_echo(cmd5); // Saut de ligne uniquement

// 	return 0;
// }


// gerer echo -nnnnnnnnnnn bonjour (affiche bonjourbash-5.1$)
//echo -n -n -n ... -l bonjour (affiche -l bonjourbash-5.1$)
//echo -nnnnnnnnl (affiche -nnnnnnl bonjour [\n]bash-5.1$)
//echo bonjour > /dev/full (affiche un message d'erreur car interdiction d'ecrire dans ce fichier)
//if (errno == ENOSPC) write message d'erreur

//echo recupere (dans cet ordre) : 1) 1 ou plsr fois l'argument -n ("-nnn" "-n -n -n")
//                                 2) un tableau de chaine de caracteres en arguments ("bonjour" "ca va" bien "-nl")

//si j'ecris juste echo, ca renvoie un retour a la ligne
