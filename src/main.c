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
// Executes a series of commands stored in the cmd_list.
// Frees the cmd_list.
{
	t_list	*current_cmd;
	// int		sep;

	current_cmd = *pipeline;
	while (current_cmd != NULL)
	{
		// sep = ((t_cmd *)current_cmd->content)->sep;
		term->status = exec_cmd(term, (t_cmd *)current_cmd->content);
		current_cmd = current_cmd->next;
		// if (term->status == 0)
		// {
		// 	if (sep == OP_OR)
		// 	{
		// 		while (((t_cmd *)current_cmd->content)->sep == OP_PIPE)
		// 			current_cmd = current_cmd->next;
		// 		current_cmd = current_cmd->next;
		// 	}
		// }
		// else
		// {
		// 	if (sep == OP_AND)
		// 	{
		// 		while (((t_cmd *)current_cmd->content)->sep == OP_PIPE)
		// 			current_cmd = current_cmd->next;
		// 		current_cmd = current_cmd->next;
		// 	}
		// }
	}
	ft_lstclear(pipeline, free_cmd);
}

void	execute_cmd_list(t_list **cmd_list, t_prog *term)
{
	t_list	*current_pipeline;

	current_pipeline = *cmd_list;
	{
		execute_pipeline((t_list **)&current_pipeline->content, term);
		if (term->status == 0)
		{
			if (((t_cmd *)((t_list *)current_pipeline->content)->content)->condition == OP_OR)
			{
				ft_lstclear(((t_list **)&current_pipeline->content), free_cmd);
				current_pipeline = current_pipeline->next;
			}
		}
		else
		{
			if (((t_cmd *)((t_list *)current_pipeline->content)->content)->condition == OP_AND)
			{
				ft_lstclear(((t_list **)&current_pipeline->content), free_cmd);
				current_pipeline = current_pipeline->next;
			}
		}
	}
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
		// connect_pipes(cmd_list);
		execute_pipeline(&cmd_list, term);
		add_history(line);
		free(line);
		line = readline(get_prompt(term, getenv_list(term->env_list, "HOME")));
	}
	handle_eof(term);
	cleanup(term);
}
