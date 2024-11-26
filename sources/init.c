/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 17:03:56 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/22 18:23:20 by ele-borg         ###   ########.fr       */
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

static void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);          // Ajoute un saut de ligne
	rl_on_new_line();           // Prépare une nouvelle ligne
	rl_replace_line("", 0);     // Efface la ligne en cours
	rl_redisplay();             // Redessine le prompt
}

void	ft_signal_handle(t_gc *gc)
{
	struct sigaction	sa;
	struct sigaction	sa_bis;

	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = &handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("Error: sigaction failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
	//	ft_error_exit("Error: sigaction failed", elements, -1, PERROR);
	memset(&sa_bis, 0, sizeof(struct sigaction));
	sa_bis.sa_handler = SIG_IGN;
	sigemptyset(&sa_bis.sa_mask);
	sa_bis.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_bis, NULL) == -1)
	{
		perror("Error: sigaction failed");
		gc_cleanup(gc);
		exit(EXIT_FAILURE);
	}
		//ft_error_exit("Error: sigaction failed", elements, -1, PERROR);
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

t_element	*ft_init_struct(t_gc *gc) //utile ? plus tard peut-etre
{
	t_element 	*elements;
	
	elements=NULL;
	elements = gc_malloc(sizeof(t_element), gc);
	elements->line = NULL;
	elements->arr = NULL;
	return (elements);
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