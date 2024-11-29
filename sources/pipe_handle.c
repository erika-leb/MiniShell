/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 14:29:23 by ele-borg          #+#    #+#             */
/*   Updated: 2024/11/29 16:36:56 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../gc/gc.h"

void	pipe_creation(t_element *elements, t_gc *gc)
{
	int	i;
	int	t;
	
	i = 0;

	while (i < elements->nb_cmd - 1)
	{
		t = pipe(elements->pipes[i]);
		if (t == -1)
		{
			perror("Error: pipe creation failed");
			gc_cleanup(gc);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	close_pipes(t_element *elements)
{
	int	k;

	k = 0;
	while (k < elements->nb_cmd - 1)
	{
		close(elements->pipes[k][1]);
		close(elements->pipes[k][0]);
		k++;
	}
}

void	part_close(t_element *elements, int k)
{
	while (k < elements->nb_cmd - 1)
	{
		close(elements->pipes[k][1]);
		close(elements->pipes[k][0]);
		k++;
	}
}

void	wait_for_children(t_element *elements)
{
	int	i;

	i = 0;
	while (i < elements->child_to_wait)
	{
		wait(NULL);
		i++;
	}
}

void	free_std(void)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}