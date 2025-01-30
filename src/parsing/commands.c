/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:18:36 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 16:56:10 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd	*construct_cmd(t_list *tokens)
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	cmd->tokens = tokens;
	cmd->fd_out = 1;
	cmd->sep = END;
	return (cmd);
}

int	is_cmd_sep(t_list *token)
{
	char	*text;

	text = (char *)token->content;
	if (ft_strncmp(text, "|", 2) == 0)
		return (PIPE);
	if (ft_strncmp(text, "||", 3) == 0)
		return (OR);
	if (ft_strncmp(text, "&&", 3) == 0)
		return (AND);
	return (0);
}

void	free_cmd(void *cmd)
{
	t_cmd	*cmdp;

	cmdp = (t_cmd *)cmd;
	ft_lstclear(&cmdp->tokens, free);
	free(cmd);
}

t_list	*split_commands(t_list *tokens)
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
		else
			current_tkn = current_tkn->next;
	}
	return (cmd_list);
}
