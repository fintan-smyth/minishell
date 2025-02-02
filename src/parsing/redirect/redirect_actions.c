/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_actions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:04:38 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/02 18:10:50 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"
#include <fcntl.h>
#include <readline/readline.h>

void	redirect_out(t_cmd *cmd, t_list **rd_token, t_list *prev, int mode)
// Applies file redirection for the redirect out (>) or append (>>) operator.
// Trims redirection tokens from the token list.
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
// Applies file redirection for the redirect in (<) operator.
// Trims redirection tokens from the token list.
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
// Prompts the user for multi line text input.
// Input stops being read when a line containing just 'delim' is found.
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
		ft_putstr_fd("minishell: warning: here-document ", 2);
		ft_putstr_fd("delimited by end-of-file (wanted ", 2);
		ft_putstr_fd(delim, 2);
		ft_putendl_fd(")", 2);
	}
	else
		free(line);
	return (hdoc);
}

void	write_hdoc(t_cmd *cmd, t_list *hdoc, t_term *term, int expand)
// Writes text input collected by the heredoc into the heredoc pipe,
// to be later read from by a command.
{
	t_list	*current;
	char	*var;

	pipe(cmd->hdpipe);
	cmd->fd_in = cmd->hdpipe[0];
	current = hdoc;
	while (current != NULL)
	{
		var = ft_strchr((char *)current->content, '$');
		if (expand && var)
			expand_var_inplace((char **)&current->content, var, term);
		ft_putendl_fd((char *)current->content, cmd->hdpipe[1]);
		current = current->next;
	}
	close(cmd->hdpipe[1]);
}

void	redirect_hdoc(t_cmd *cmd, t_list **rd_token, t_list *prev, t_term *term)
// Applies file redirection for the heredoc (<<) operator.
// Trims redirection tokens from the token list.
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
