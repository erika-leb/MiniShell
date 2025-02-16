/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_bis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ele-borg <ele-borg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 16:36:36 by ele-borg          #+#    #+#             */
/*   Updated: 2025/02/16 17:43:33 by ele-borg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reset_signal_status(void)
{
	rl_done = 0;
	g_signal = 0;
}

void	handle_sigpipe(int sig)
{
	g_signal = sig;
}
