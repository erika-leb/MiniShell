# include <stdio.h>//printf
# include <stdlib.h>//free, ...
# include <stdint.h>//SIZE_MAX
# include <signal.h>//sigaction
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>

# define	TRUE		1
# define	FALSE		0

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

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
void    ft_fill_buf(int *i, int *k, char **cmd, char *buf)
{
	int j;

	j = 0;
	while (cmd[*i][j])
		buf[(*k)++] = cmd[*i][j++];
	(*i)++;
}

void    ft_echo(char **cmd)
{
	int i;
	int j;
	int k;
	int n;
	char buf[70000];

	i = 1;
	k = 0;
	n = 0;
	if (!cmd[1])
	{
		write(1, "\n", 1);
		return ;
	}
	if (ft_is_option(cmd[i]) == TRUE)
		n = 1;
	while (cmd[i] && ft_is_option(cmd[i]) == TRUE)
		i++;
	while (cmd[i])
		ft_fill_buf(&i, &k, cmd, buf);
	if (n == 0)
		buf[k++] = '\n';
	write(1, buf, k); //a verifier le 1
	if (errno == ENOSPC) // a veririfer a la fin
		perror("echo: write error:");
}

int main() {
	// Test 1 : Avec l'option -n
	char *cmd1[] = {"echo", "-n", "Hello", "World!", NULL};
	ft_echo(cmd1); // "Hello World!" sans saut de ligne

	// Test 2 : Sans l'option -n
	char *cmd2[] = {"echo", "Hello", "there", "friend!", NULL};
	ft_echo(cmd2); // "Hello there friend!" avec saut de ligne

	// Test 3 : Seulement l'option -n
	char *cmd3[] = {"echo", "-n", NULL};
	ft_echo(cmd3); // Aucun message, pas de saut de ligne

	// Test 4 : Plusieurs options -n
	char *cmd4[] = {"echo", "-n", "-n", "Hello", "again", NULL};
	ft_echo(cmd4); // "Hello again" sans saut de ligne

	// Test 5 : Aucun argument (uniquement echo)
	char *cmd5[] = {"echo", "-nnnnnnnnj", "ca", NULL};
	ft_echo(cmd5); // Saut de ligne uniquement

	return 0;
}


// gerer echo -nnnnnnnnnnn bonjour (affiche bonjourbash-5.1$)
//echo -n -n -n ... -l bonjour (affiche -l bonjourbash-5.1$)
//echo -nnnnnnnnl (affiche -nnnnnnl bonjour [\n]bash-5.1$)
//echo bonjour > /dev/full (affiche un message d'erreur car interdiction d'ecrire dans ce fichier)
//if (errno == ENOSPC) write message d'erreur

//echo recupere (dans cet ordre) : 1) 1 ou plsr fois l'argument -n ("-nnn" "-n -n -n")
//                                 2) un tableau de chaine de caracteres en arguments ("bonjour" "ca va" bien "-nl")

//si j'ecris juste echo, ca renvoie un retour a la ligne
