/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:32:29 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/02 18:07:16 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"
# include <fcntl.h>

typedef struct s_cmd
{
	t_list	*tokens;
	char	**argv;
	int		argc;
	int		sep;
	int		fd_in;
	int		fd_out;
	int		pipe[2];
	int		hdpipe[2];
	int		error;
}	t_cmd;

enum
{
	OP_END = 0,
	OP_PIPE = 1,
	OP_AND = 2,
	OP_OR = 3,
};

enum
{
	Q_NONE = 0,
	Q_SINGLE,
	Q_DOUBLE,
};

enum
{
	RD_IN = 1,
	RD_OUT = 2,
	RD_APP = 3,
	RD_HRD = 4,
};

t_list	*parse_line(char *line, t_term *term);

//Tokenise
int		is_op(char c);
int		matches_prev_op(char c, char prev);
int		apply_quoting_tokenise(int *quoting, char c, char *q_char);
void	print_tokens(t_list *tokens);
t_list	*tokenise(char *line);

//Expansion
char	*extend_line(char *line, char *extra);
int		expand_var_inplace(char **line, char *varp, t_term *term);
void	expand_token_var(char **token, t_term *term);
void	expand_wildcards(char **line, t_term *term);
void	expand_token_list(t_list *tokens, t_term *term);

//Stripping
void	strip_quotes_token(char *token);
void	strip_quotes(t_list **tokens);

//Commands
void	free_cmd(void *cmd);
int		is_cmd_sep(t_list *token);
t_list	*split_commands(t_list *tokens);

//Redirection
int		is_redirect(t_list *token);
void	encode_redirect(t_list *token);
void	redirect_hdoc(t_cmd *cmd, t_list **rd_token,
			t_list *prev, t_term *term);
void	redirect_out(t_cmd *cmd, t_list **rd_token, t_list *prev, int mode);
void	redirect_in(t_cmd *cmd, t_list **rd_token, t_list *prev);
void	apply_redirection(t_cmd *cmd, t_term *term);
void	connect_pipes(t_list *cmd_list);

#endif // PARSING_H
