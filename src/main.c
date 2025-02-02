/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:50:15 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/02 17:37:09 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parsing.h"
#include <readline/history.h>
#include <readline/readline.h>

void	exec_cmd(t_term *term, t_cmd *cmd);

void	cleanup(t_term *term)
{
	ft_lstclear(&term->env_list, free_env);
	rl_clear_history();
	free(term->prompt);
	free(term);
}

t_term	*init_term(char *name, char **line)
{
	t_term	*term;

	term = ft_calloc(1, sizeof(*term));
	getcwd(term->cwd, PATH_MAX);
	init_env_list(term, name);
	*line = readline(get_prompt(term, getenv_list(term->env_list, "HOME")));
	return (term);
}

void	execute_cmd_list(t_list **cmd_list, t_term *term, char *line)
{
	t_list	*current_cmd;

	current_cmd = *cmd_list;
	while (current_cmd != NULL)
	{
		exec_cmd(term, (t_cmd *)current_cmd->content);
		current_cmd = current_cmd->next;
	}
	add_history(line);
	free(line);
	ft_lstclear(cmd_list, free_cmd);
}

int	main(int argc, char **argv)
{
	t_term	*term;
	t_list	*cmd_list;
	char	*line;

	(void)argc;
	term = init_term(argv[0], &line);
	while (line != NULL)
	{
		if (*line == 0)
		{
			free(line);
			line = readline(get_prompt(term,
						getenv_list(term->env_list, "HOME")));
			continue ;
		}
		cmd_list = parse_line(line, term);
		connect_pipes(cmd_list);
		execute_cmd_list(&cmd_list, term, line);
		line = readline(get_prompt(term, getenv_list(term->env_list, "HOME")));
	}
	cleanup(term);
}
