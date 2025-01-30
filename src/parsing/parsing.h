/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:32:29 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 17:31:39 by fsmyth           ###   ########.fr       */
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
	Q_NONE,
	Q_SINGLE,
	Q_DOUBLE,
};

enum
{
	RD_IN = 1,
	RD_OUT = 2,
	RD_APP = 3,
	RD_HERED = 4,

};

t_list	*tokenise(char *line);
void	**lst_to_arr(t_list *list);
void	print_tokens(t_list *tokens);
void	expand_token(char **token, t_term *term);
void	expand_token_list(t_list *tokens, t_term *term);
void	strip_quotes(t_list **tokens);
void	free_cmd(void *cmd);
int		is_cmd_sep(t_list *token);
t_list	*split_commands(t_list *tokens);
int		is_redirect(t_list *token);
void	encode_redirect(t_list *token);
void	apply_redirection(t_cmd *cmd);

#endif // PARSING_H
