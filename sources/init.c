/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:03:56 by ele-borg          #+#    #+#             */
/*   Updated: 2025/01/30 18:57:25 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

void	ft_welcome(void)
{
	printf("\n");
	printf(CCYAN "******************************\n" CRESET);
	printf(CMAGENTA "*  " CYELLOW "Minishell" CMAGENTA "  *\n" CRESET);
	printf(CCYAN "******************************\n" CRESET);
	printf("\n");
}

void	ft_cpy_env(t_element *elements, char **env, t_gc *gc, char **argv)
{
	int		s_arr;
	int		i;
	int		s;

	if (env[0] == NULL)
		s_arr = 3;
	else
		s_arr = ft_arr_size(env);
	elements->env = gc_malloc(sizeof(char *) * (s_arr + 1), gc);
	i = 0;
	// printf("%s\n", argv[0]);
	if (env[0] == NULL)
	{
		elements->env[0] = ft_strjoin_("PWD=", getcwd(NULL, 0), gc);//getcwd(NULL, 0);
		elements->env[1] = ft_strdup_("SHLVL=1", gc);
		elements->env[2] = ft_strjoin_("_=", argv[0], gc);//ft_strdup_(argv[0], gc);
		elements->env[3] = NULL;
	}
	else
	{
		while (env[i])
		{
			s = ft_strlen(env[i]);
			elements->env[i] = gc_malloc(s + 1, gc);
			elements->env[i] = env[i];
			i++;
		}
		elements->env[i] = NULL;
		ft_handle_path(elements, gc);
	}
	// i = 0;
	// while( i <= s_arr)
	// {
	// 	//printf("env %i = %s\n", i, env[i]);
	// 	printf("myenv %i = %s\n", i, elements->env[i]);
	// 	i++;
	// }
}


t_element	*ft_init_struct(t_gc *gc)
{
	t_element 	*elements;

	elements = NULL;
	elements = gc_malloc(sizeof(t_element), gc);
	elements->line = NULL;
	elements->arr = NULL;
	elements->lst = NULL;
	elements->mypaths = NULL;
	elements->nb_cmd = 0;
	elements->pid_arr = NULL;
	elements->pipes = NULL;
	elements->child_to_wait = 0;
	elements->error = NULL;
	elements->env = NULL;
	// elements->err = 0;
	elements->exit_status = ft_itoa(0, gc);
	elements->line_valid = TRUE;
 	return (elements);
}


void gc_print(t_gc *gc)
{
    t_gc_node *current = gc->head;

    if (current == NULL)
    {
        printf("GC is empty, no allocations tracked.\n");
        return;
    }

    printf("GC status:\n");

    while (current != NULL)
    {
        printf("Allocation at address %p, size: %zu bytes\n", current->ptr, current->size);
        current = current->next;
    }
}




/*void	ft_error_exit(char *s, t_element *elements, int	i, int type)
{
	if (elements->line != NULL)
		free(elements->line); // mettre a NULL ??
	if (elements->arr != NULL)
		ft_freesplit(elements->arr, ft_arr_size(elements->arr)); // mettre a NULL ?
	if (type == PERROR)
		perror(s);
	else if (ft_strcmp(s, "") != 0)
		printf("%s\n", s);
	clear_history();
	free(elements);
	exit(i);
}*/
