/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:50:15 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/14 15:18:31 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parsing.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/wait.h>

void	cleanup(t_prog *term)
// Cleans up the t_prog struct before exit
{
	ft_lstclear(&term->env_list, free_env);
	rl_clear_history();
	free(term->prompt);
	free(term);
}

int	main(int argc, char **argv, char *env[])
{
	t_prog	*term;
	t_ptree	*ptree;
	char	*line;

	term = start_program(argc, argv, env, &line);
	while (line != NULL)
	{
		if (*line == 0 || ft_strwhitespace(line))
		{
			free(line);
			line = readline(get_prompt(term,
						getenv_list(term->env_list, "HOME")));
			continue ;
		}
		term->line = line;
		ptree = parse_line(line, term);
		if (term->parse_status == 0)
			execute_ptree(ptree, term);
		traverse_ptree(ptree, PST_ORD, free_ptree_node, NULL);
		term->ptree = NULL;
		add_history(line);
		free(line);
		line = readline(get_prompt(term, getenv_list(term->env_list, "HOME")));
	}
	handle_eof(term);
}
