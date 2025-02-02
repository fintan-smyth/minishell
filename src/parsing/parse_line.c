/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:10:22 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/03 20:27:30 by myiu             ###   ########.fr       */
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

t_list	*parse_line(char *line, t_prog *term)
// Parses a line into a list of commands to be executed, applying any
// necessary expansion and redirection.
{
	t_list	*tokens;
	t_list	*cmd_list;
	t_list	*current;

	tokens = tokenise(line);
	cmd_list = split_commands(tokens);
	current = cmd_list;
	while (current != NULL)
	{
		tokens = ((t_cmd *)current->content)->tokens;
		if (is_debug(term))
			print_parse_debug(tokens, (t_cmd *)current->content, "Tokenised");
		expand_token_list(tokens, term);
		if (is_debug(term))
			print_parse_debug(tokens, (t_cmd *)current->content, "Expanded");
		strip_quotes(&tokens);
		if (is_debug(term))
			print_parse_debug(tokens, (t_cmd *)current->content, "Stripped");
		apply_redirection((t_cmd *)current->content, term);
		if (is_debug(term))
			print_parse_debug(tokens, (t_cmd *)current->content, "Redirected");
		prepare_args((t_cmd *)current->content);
		current = current->next;
	}
	return (cmd_list);
}
