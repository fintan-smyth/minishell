/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:19:48 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 19:13:49 by fsmyth           ###   ########.fr       */
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

void	redirect_out(t_cmd *cmd, t_list *rd_token, int mode)
{
	int		fd;
	int		fmode;
	t_list	*prev;

	if (rd_token->next == NULL)
	{
		cmd->error = 1;
		return ;
	}
	if (mode == RD_APP)
		fmode = O_APPEND;
	else
		fmode = O_WRONLY;
	fd = open((char *)rd_token->next->content, (fmode | O_CREAT));
	ft_printf("fd: %d\n", fd);
	cmd->fd_out = fd;
	prev = cmd->tokens;
	while (prev->next != rd_token)
		prev = prev->next;
	prev->next = NULL;
	ft_lstdelone(rd_token->next, free);
	ft_lstdelone(rd_token, free);
}

void	apply_redirection(t_cmd *cmd)
{
	t_list	*current;
	int		mode;

	current = cmd->tokens;
	while (current != NULL)
	{
		mode = *(char *)current->content;
		if (mode == RD_IN)
			;
		else if (mode == RD_OUT || mode == RD_APP)
		{
			redirect_out(cmd, current, mode);
			break ;
		}
		else if (mode == RD_HERED)
			;
		current = current->next;
	}
}
