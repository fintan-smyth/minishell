/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:50:15 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/07 15:42:05 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parsing.h"
#include <readline/history.h>
#include <readline/readline.h>

int	exec_cmd(t_prog *term, t_cmd *cmd, t_list *pipeline);

void	cleanup(t_prog *term)
// Cleans up the t_prog struct before exit
{
	ft_lstclear(&term->env_list, free_env);
	rl_clear_history();
	free(term->prompt);
	free(term);
}

t_prog	*init_term(char *name, char **line, char **env)
// Initialises a t_prog structure
{
	t_prog			*prog;
	struct termios	term;

	prog = ft_calloc(1, sizeof(*prog));
	prog->status = 0;
	getcwd(prog->cwd, PATH_MAX);
	init_env_list(prog, name, env);
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(fileno(stdin), TCSANOW, &term);
	*line = readline(get_prompt(prog, getenv_list(prog->env_list, "HOME")));
	return (prog);
}

void	execute_pipeline(t_list *pipeline, t_prog *term)
// Executes a series of commands that form a pipeline.
// Frees the cmd_list.
{
	t_list	*current_cmd;
	int		i;

	current_cmd = pipeline;
	i = 0;
	while (current_cmd != NULL)
	{
		if (is_debug(term))
			ft_printf("\e[1;33m### EXECUTING SUBCMD No %d ###\e[m\n", ++i);
		term->status = exec_cmd(term, (t_cmd *)current_cmd->content, pipeline);
		current_cmd = current_cmd->next;
	}
}

void	execute_ptree(t_ptree *ptree, t_prog *term)
{
	if (ptree == NULL)
		return ;
	execute_ptree(ptree->left, term);
	if (ptree->op == 0)
		execute_pipeline(ptree->pipeline, term);
	else if (ptree->op == OP_AND && term->status != 0)
		return ;
	else if (ptree->op == OP_OR && term->status == 0)
		return ;
	execute_ptree(ptree->right, term);
}

int	main(int argc, char **argv, char *env[])
{
	t_prog	*term;
	t_ptree	*ptree;
	char	*line;
	int		i;

	(void)argc;
	i = 0;
	setup_signals();
	term = init_term(argv[0], &line, env);
	while (line != NULL)
	{
		if (*line == 0)
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
	cleanup(term);
}
