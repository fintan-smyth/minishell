/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:24:55 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/05 23:25:08 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>

sig_atomic_t g_signal;

/*
	ctrl C(sigint) -> new prompt on a new line
	ctrl D(EOF) -> exit and printf
	ctrl \(sigquit) -> clean up, do nth
*/
void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}


void	setup_signals(void)
{
	struct sigaction	sa;

	g_signal = 0;
	sa.sa_handler = sig_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_eof(t_prog *term)
{
	printf("exit\n");
	cleanup(term);
	exit(EXIT_SUCCESS);
}

void	reset_child_sig(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
