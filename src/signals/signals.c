#include "../minishell.h"
#include <signal.h>

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
