/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:19:48 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/31 00:16:38 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <fcntl.h>

int	is_redirect(t_list *token)
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
		return (RD_HERED);
	return (0);
}

void	encode_redirect(t_list *token)
{
	int		mode;

	mode = is_redirect(token);
	free(token->content);
	token->content = ft_strdup(" ");
	*(char *)token->content = mode;
}

void	redirect_out(t_cmd *cmd, t_list **rd_token, t_list *prev, int mode)
{
	int		fd;
	int		fmode;

	if ((*rd_token)->next == NULL || is_redirect((*rd_token)->next))
	{
		cmd->error = 1;
		return ;
	}
	fmode = O_WRONLY | O_CREAT;
	if (mode == RD_APP)
		fmode |= O_APPEND;
	else
		fmode |= O_TRUNC;
	fd = open((char *)(*rd_token)->next->content, fmode, 0644);
	if (fd < 0)
	{
		cmd->error = 2;
		return ;
	}
	if (cmd->fd_out > 2)
		close(cmd->fd_out);
	cmd->fd_out = fd;
	ft_lstdel_next(prev, free);
	ft_lstdel_next(prev, free);
	*rd_token = NULL;
}

void	redirect_in(t_cmd *cmd, t_list **rd_token, t_list *prev)
{
	int		fd;

	if ((*rd_token)->next == NULL || is_redirect((*rd_token)->next))
	{
		cmd->error = 1;
		return ;
	}
	fd = open((char *)(*rd_token)->next->content, O_RDONLY);
	if (fd < 0)
	{
		cmd->error = 2;
		return ;
	}
	if (cmd->fd_in > 2)
		close(cmd->fd_in);
	cmd->fd_in = fd;
	ft_lstdel_next(prev, free);
	ft_lstdel_next(prev, free);
	*rd_token = NULL;
}

void	apply_redirection(t_cmd *cmd)
{
	t_list	*current;
	t_list	*prev;
	int		mode;

	current = cmd->tokens;
	prev = NULL;
	while (current != NULL)
	{
		mode = *(char *)current->content;
		if (mode == RD_IN)
			redirect_in(cmd, &current, prev);
		else if (mode == RD_OUT || mode == RD_APP)
			redirect_out(cmd, &current, prev, mode);
		// else if (mode == RD_HERED)
		// 	;
		if (current == NULL)
			current = prev->next;
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
