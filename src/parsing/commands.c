/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:18:36 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/01 14:52:08 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd	*construct_cmd(t_list *tokens)
// Returns a memory allocated t_cmd struct containing 'tokens'.
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	cmd->tokens = tokens;
	cmd->fd_out = 1;
	(cmd->pipe)[0] = -1;
	(cmd->pipe)[1] = -1;
	cmd->sep = OP_END;
	return (cmd);
}

int	is_cmd_sep(t_list *token)
// Checks if a token is used to separate commands.
{
	char	*text;

	text = (char *)token->content;
	if (ft_strncmp(text, "|", 2) == 0)
		return (OP_PIPE);
	if (ft_strncmp(text, "||", 3) == 0)
		return (OP_OR);
	if (ft_strncmp(text, "&&", 3) == 0)
		return (OP_AND);
	return (0);
}

void	free_cmd(void *cmdptr)
// Frees a t_cmd struct.
{
	t_cmd	*cmd;

	cmd = (t_cmd *)cmdptr;
	ft_lstclear(&cmd->tokens, free);
	free(cmd->argv);
	if (cmd->rd_in == 1)
		close(cmd->fd_in);
	if (cmd->rd_out == 1)
		close(cmd->fd_out);
	free(cmdptr);
}

t_list	*split_commands(t_list *tokens)
// Splits a list of tokens into a list of commands to be executed in sequence.
// Returns the list of t_cmd structs.
{
	t_list	*cmd_list;
	t_list	*current_tkn;
	t_cmd	*cmd;
	int		sep;

	cmd_list = NULL;
	cmd = construct_cmd(tokens);
	ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
	current_tkn = tokens;
	while (current_tkn != NULL)
	{
		sep = is_cmd_sep(current_tkn);
		if (sep && current_tkn->next != NULL)
		{
			cmd->sep = sep;
			cmd = construct_cmd(current_tkn->next);
			ft_lstadd_back(&cmd_list, ft_lstnew(cmd));
			current_tkn->next = NULL;
			current_tkn = cmd->tokens;
		}
		else if (is_redirect(current_tkn))
			encode_redirect(current_tkn);
		current_tkn = current_tkn->next;
	}
	return (cmd_list);
}
