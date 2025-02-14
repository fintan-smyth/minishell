/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:11:24 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/14 16:01:22 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_prog	*init_term(char **line, char **env)
// Initialises a t_prog structure
{
	t_prog			*prog;
	struct termios	term;

	prog = ft_calloc(1, sizeof(*prog));
	prog->status = 0;
	getcwd(prog->cwd, PATH_MAX);
	init_env_list(prog, env);
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(fileno(stdin), TCSANOW, &term);
	*line = readline(get_prompt(prog, getenv_list(prog->env_list, "HOME")));
	return (prog);
}

t_prog	*start_program(int argc, char **argv, char *env[], char **line)
{
	t_prog	*term;

	(void)argc;
	(void)argv;
	setup_signals();
	term = init_term(line, env);
	return (term);
}
