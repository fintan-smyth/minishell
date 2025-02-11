/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:19:48 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/05 23:04:05 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"
#include <fcntl.h>
#include <readline/readline.h>

int	is_redirect(t_list *token)
// Checks if a token is an unencoded redirect operator.
// If it is, returns an int corresponding tow which operator it is.
{
	char	*text;

	text = (char *)token->content;
	if (ft_strncmp(text, "<", 2) == 0)
		return (RD_IN);
	if (ft_strncmp(text, ">", 2) == 0)
		return (RD_OUT);
	if (ft_strncmp(text, ">>", 3) == 0)
		return (RD_APP);
	if (ft_strncmp(text, "<<", 3) == 0)
		return (RD_HRD);
	return (0);
}

void	encode_redirect(t_list *token)
// Replaces valid redirect operators with an encoded character, to
// differentiate from characters blocked by quoting orotherwise not
// to be expanded
{
	int		mode;

	mode = is_redirect(token);
	free(token->content);
	token->content = ft_strdup(" ");
	*(char *)token->content = mode;
}

void	apply_redirection(t_cmd *cmd, t_prog *term)
// Executes all redirection for a command.
{
	t_list	*current;
	t_list	*prev;
	int		mode;

	handle_hdocs(cmd, term);
	print_tokens(cmd->tokens);
	current = cmd->tokens;
	prev = NULL;
	while (current != NULL && term->parse_status == 0 && cmd->rd_err == 0)
	{
		mode = *(char *)current->content;
		if (mode == RD_IN)
			redirect_in(cmd, &current, prev);
		else if (mode == RD_OUT || mode == RD_APP)
			redirect_out(cmd, &current, prev, mode);
		else if (mode == RD_HRD)
			redirect_hdoc(cmd, &current, prev);
		if (current == NULL)
			current = prev->next;
		else
		{
			prev = current;
			current = current->next;
		}
	}
	if (cmd->fd_in != cmd->hdpipe[0] && cmd->hdpipe[0] > 2)
		close(cmd->hdpipe[0]);
}
