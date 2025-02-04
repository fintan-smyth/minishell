/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:50:15 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/03 20:27:30 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parsing.h"
#include <readline/history.h>
#include <readline/readline.h>

int	exec_cmd(t_prog *term, t_cmd *cmd);

void	cleanup(t_prog *term)
// Cleans up the t_prog struct before exit
{
	ft_lstclear(&term->env_list, free_env);
	rl_clear_history();
	free(term->prompt);
	free(term);
}

t_prog	*init_term(char *name, char **line)
// Initialises a t_prog structure
{
	t_prog			*prog;
	struct termios	term;

	prog = ft_calloc(1, sizeof(*prog));
	prog->status = 0;
	getcwd(prog->cwd, PATH_MAX);
	init_env_list(prog, name);
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(fileno(stdin), TCSANOW, &term);
	*line = readline(get_prompt(prog, getenv_list(prog->env_list, "HOME")));
	return (prog);
}

void	execute_pipeline(t_list **pipeline, t_prog *term)
// Executes a series of commands that form a pipeline.
// Frees the cmd_list.
{
	t_list	*current_cmd;
	int		i;

	current_cmd = *pipeline;
	i = 0;
	while (current_cmd != NULL)
	{
		if (is_debug(term))
			ft_printf("\e[1;33m### SUBCMD No %d ###\e[m\n", ++i);
		term->status = exec_cmd(term, (t_cmd *)current_cmd->content);
		current_cmd = current_cmd->next;
	}
}

void	execute_cmd_list(t_list **cmd_list, t_prog *term)
{
	t_list	*current_pipeline;
	int	i;

	current_pipeline = *cmd_list;
	i = 0;
	term->status = 0;
	while (current_pipeline != NULL)
	{
		if (term->status == 0)
		{
			if (((t_cmd *)((t_list *)current_pipeline->content)->content)->condition == OP_OR)
			{
				current_pipeline = current_pipeline->next;
				i++;
				continue ;
			}
		}
		else
		{
			if (((t_cmd *)((t_list *)current_pipeline->content)->content)->condition == OP_AND)
			{
				current_pipeline = current_pipeline->next;
				i++;
			}
		}
		if (is_debug(term))
			ft_printf("\e[1;33m### PIPELINE No %d ###\e[m\n", ++i);
		execute_pipeline((t_list **)&current_pipeline->content, term);
		current_pipeline = current_pipeline->next;
	}
	ft_lstclear(cmd_list, free_pipeline);
}

int	main(int argc, char **argv)
{
	t_prog	*term;
	t_list	*cmd_list;
	char	*line;

	(void)argc;
	setup_signals();
	term = init_term(argv[0], &line);
	while (line != NULL)
	{
		//setup_signals();
		if (*line == 0)
		{
			free(line);
			line = readline(get_prompt(term,
						getenv_list(term->env_list, "HOME")));
			continue ;
		}
		cmd_list = parse_line(line, term);
		connect_pipes(cmd_list);
		execute_cmd_list(&cmd_list, term);
		add_history(line);
		free(line);
		line = readline(get_prompt(term, getenv_list(term->env_list, "HOME")));
	}
	handle_eof(term);
	cleanup(term);
}
