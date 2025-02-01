/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:19:48 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/01 14:25:05 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <fcntl.h>
#include <readline/readline.h>

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
		return (RD_HRD);
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

t_list	*read_hdoc(char *delim)
{
	char	*line;
	t_list	*hdoc;

	hdoc = NULL;
	line = readline(">");
	while (line != NULL && ft_strncmp(line, delim, ft_strlen(delim) + 1))
	{
		ft_lstadd_back(&hdoc, ft_lstnew(line));
		line = readline(">");
	}
	if (line == NULL)
	{
		ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted ", 2);
		ft_putstr_fd(delim, 2);
		ft_putendl_fd(")", 2);
	}
	else
		free(line);
	return (hdoc);
}

void	write_hdoc(t_cmd *cmd, t_list *hdoc, t_term *term, int expand)
{
	t_list	*current;
	char	*var;

	pipe(cmd->pipe);
	cmd->fd_in = cmd->pipe[0];
	current = hdoc;
	while (current != NULL)
	{
		var = ft_strchr((char *)current->content, '$');
		if (expand && var)
			expand_var_inplace((char **)&current->content, var, term);
		ft_putendl_fd((char *)current->content, cmd->pipe[1]);
		current = current->next;
	}
	// close(cmd->pipe[1]);
}

void	redirect_hdoc(t_cmd *cmd, t_list **rd_token, t_list *prev, t_term * term)
{
	char	*delim;
	int		expand;
	t_list	*hdoc;

	expand = 1;
	delim = (char *)(*rd_token)->next->content;
	if (ft_strchr(delim, '\"') || ft_strchr(delim, '\''))
	{
		expand = 0;
		strip_quotes_token(delim);
	}
	hdoc = read_hdoc(delim);
	write_hdoc(cmd, hdoc, term, expand);
	ft_lstclear(&hdoc, free);
	ft_lstdel_next(prev, free);
	ft_lstdel_next(prev, free);
	*rd_token = NULL;
}

void	apply_redirection(t_cmd *cmd, t_term *term)
{
	t_list	*current;
	t_list	*prev;
	int		mode;

	(void)term;
	current = cmd->tokens;
	prev = NULL;
	while (current != NULL)
	{
		mode = *(char *)current->content;
		if (mode == RD_IN)
			redirect_in(cmd, &current, prev);
		else if (mode == RD_OUT || mode == RD_APP)
			redirect_out(cmd, &current, prev, mode);
		else if (mode == RD_HRD)
		{
			redirect_hdoc(cmd, &current, prev, term);
		}
		if (current == NULL)
			current = prev->next;
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
