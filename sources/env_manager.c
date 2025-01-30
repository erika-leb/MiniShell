/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 14:28:51 by aisidore          #+#    #+#             */
/*   Updated: 2025/01/30 17:56:38 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

static t_env *ft_filltoa(char **array, t_env *current, size_t i, t_gc *gc)
{
	size_t  j;
	size_t  len;

	if (current->key)
		len = ft_strlen(current->name) + ft_strlen(current->key) + 2;
	else
		len = ft_strlen(current->name) + 2;
	array[i] = gc_malloc(len * sizeof(char), gc);
	j = -1;
	while (current->name[++j])
		array[i][j] = current->name[j];
	array[i][j] = '\0';//useless avec gc_calloc I guess
	if (current->key)
		ft_strcat(ft_strcat(array[i], "="), current->key);
	return (current->next);
}

//Il faudrait proteger cette fonction pour eviter les erreurs chelous
char **ft_ltoa(t_env *head, t_gc *gc)
{
	size_t  count;
	size_t  i;
	t_env   *current;
	char    **array;

	if (!head)
		return NULL;
	current = head;
	count = 0;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = gc_calloc(count + 1, sizeof(char *), gc);
	current = head;
	i = 0;
	while (i < count)
		current = ft_filltoa(array, current, i++, gc);
	return (array);
}
//Je l'utilise plus je crois
void ft_freetab(char **array)
{
	int j;

	j = -1;
	while (array[++j])
		free(array[j]);
	free(array);

}

static int	ft_merrorexp(char *name_key, t_element *elements, t_gc *gc)
{
	ft_buff_error("minishell: export: `", elements, gc);
	ft_buff_error(name_key, elements, gc);
	ft_buff_error("': not a valid identifier\n", elements, gc);
	ft_write_error(elements, gc);
	return (1);
}
//De facon general il faut bien faire gaffe au comportement de minishell
//si le user s'amuse a mettre les options n'importe ou dans cmds[i] ?
int   ft_exparser(char *name_key, t_element *elements, t_gc *gc)
{
	int i;

	(void) elements;/////
	(void) gc;///////

	//ft_write de Erika a ajouter
	if (!ft_isalpha(name_key[0]) && name_key[0] != '_')
		return (ft_merrorexp(name_key, elements, gc)); //changer le message et le exit status
	i = 0;//on check a partir du 2eme caractere c'est pourquoi on met i = 0.
	while (name_key[++i])
	{
		if (name_key[i] == '+' && name_key[i + 1] == '=')
			break;
		if (name_key[i] == '=')
			break;
		if (!ft_isalnum(name_key[i]) && name_key[i] != '_')
			return(ft_merrorexp(name_key, elements, gc));
	}
	return (0);
}

static char	*ft_strchr(char const *str, int c)
{
	unsigned int	i;
	char			char_c;

	i = 0;
	char_c = (char)c;
	while (str[i] != '\0')
	{
		if (str[i] == char_c)
			return ((char *)&str[i]);
		i++;
	}
	if (str[i] == char_c)
		return ((char *)&str[i]);
	return (NULL);
}

void ft_env(char **array, char **cmds, t_gc *gc)
{
	int		i;
	int		size;
	char	*res;

	if (cmds && cmds[1])//cmds && servait juste a le tester dans mon main
	{//parsing. cmds[0] = env     cmds[1...] = le reste ...
		write(2,"minshell: env: No option(s) or argument(s) allowed\n", 52);//ft_write
		if (cmds[1][0] == '-')
			(gc_cleanup(gc), free_std(), exit(125));
		(gc_cleanup(gc), free_std(), exit(127));
	}
	i = 0;
	while (array[i])
	{
		if (ft_strchr(array[i],'='))
		{
			res = ft_strjoin_(array[i], "\n", gc);
			size = ft_strlen(res);
			write(1, res, size);
		}
			//printf("%s\n", array[i]);//ft_write
		i++;
	}
	(gc_cleanup(gc), free_std(), exit(EXIT_SUCCESS));
}
