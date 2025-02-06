/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:10:22 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/06 16:31:32 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parsing.h"

void	print_parse_debug(t_list *tokens, t_cmd *cmd, char *msg)
// Prints debugging output if MS_DEBUG environemnt variable is set to ON.
{
	ft_printf("\e[1;3%dm%s\n\e[m", *msg % 6 + 1, msg);
	print_tokens(tokens);
	if (ft_strncmp(msg, "Redirected", 11) == 0)
	{
		ft_printf("fd_out\t%d\nfd_in:\t%d\nerr:\t%d\n",
			cmd->fd_out,
			cmd->fd_in,
			cmd->error);
		if (cmd->sep == OP_PIPE)
			ft_printf("\e[1;32mvvvvvvvv\nPIPED TO\nvvvvvvvv\e[m\n");
		else
			ft_printf("----------\n");
	}
}

int	is_debug(t_prog *term)
// Checks if MS_DEBUG environment variable is set to ON.
{
	char	*debug;

	debug = getenv_list(term->env_list, "MS_DEBUG");
	if (debug == NULL)
		return (0);
	else if (ft_strncmp(debug, "ON", 3) == 0)
		return (1);
	return (0);
}

void	prepare_args(t_cmd *cmd)
// Places 'argc' and 'argv' variables into t_cmd struct to be
// used during execution.
{
	cmd->argv = (char **)ft_lst_to_arr(cmd->tokens);
	cmd->argc = count_args(cmd->argv);
}

//changed current_cmd to cur_cmd for norm
void	parse_pipeline(t_ptree *ptree, void *term)
{
	t_list	*cur_cmd;
	t_list	*tokens;

	if (ptree->op != 0)
		return ;
	cur_cmd = ptree->pipeline;
	while (cur_cmd != NULL && ((t_prog *)term)->parse_status == 0)
	{
		tokens = ((t_cmd *)cur_cmd->content)->tokens;
		if (is_debug((t_prog *)term))
			print_parse_debug(tokens, (t_cmd *)cur_cmd->content, "Tokenised");
		expand_token_list(tokens, (t_prog *)term);
		if (is_debug((t_prog *)term))
			print_parse_debug(tokens, (t_cmd *)cur_cmd->content, "Expanded");
		strip_quotes(&tokens);
		if (is_debug((t_prog *)term))
			print_parse_debug(tokens, (t_cmd *)cur_cmd->content, "Stripped");
		apply_redirection((t_cmd *)cur_cmd->content, (t_prog *)term);
		if (is_debug((t_prog *)term))
			print_parse_debug(tokens, (t_cmd *)cur_cmd->content, "Redirected");
		prepare_args((t_cmd *)cur_cmd->content);
		cur_cmd = cur_cmd->next;
	}
	connect_pipes(ptree->pipeline);
}

t_ptree	*parse_line(char *line, t_prog *term)
// Parses a line into a list of commands to be executed, applying any
// necessary expansion and redirection.
{
	t_list	*tokens;
	t_list	*cmd_list;

	term->parse_status = 0;
	tokens = tokenise(line);
	cmd_list = split_commands(tokens);
	term->ptree = construct_parse_tree(&cmd_list);
	traverse_ptree(term->ptree, IN_ORD, parse_pipeline, term);
	if (is_debug(term))
	{
		ft_printf("\e[35m### POST ORDER ###\e[m\n");
		traverse_ptree(term->ptree, PST_ORD, print_ptree_node, NULL);
		write(1, "\n", 1);
	}
	return (term->ptree);
}
