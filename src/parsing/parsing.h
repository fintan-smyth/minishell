/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:32:29 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/08 17:15:37 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"
# include <fcntl.h>
# include <stdbool.h>
# include "../structs.h"

enum
{
	OP_NONE = 0,
	OP_PIPE = 1,
	OP_AND = 2,
	OP_OR = 3,
	OP_OPNPRN = 4,
	OP_CLSPRN = 5,
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

enum
{
	IN_ORD,
	PRE_ORD,
	PST_ORD,
};

//Tokenise
int		is_op(char c);
int		token_is_op(t_list *token);
int		matches_prev_op(char c, char prev);
int		apply_quoting_tokenise(int *quoting, char c, char *q_char);
void	print_tokens(t_list *tokens);
t_list	*tokenise(char *line);

//Syntax
int		verify_tkn_syntax(t_list *tokens, t_prog *term);

//Expansion
int		valid_var_chr(char c);
char	*extend_line(char *line, char *extra);
int		expand_var_inplace(char **line, char *varp, t_prog *term);
void	expand_token_var(char **token, t_prog *term);
void	expand_wildcards(char **line, t_prog *term);
void	expand_token_list(t_list *tokens, t_prog *term);

//Stripping
void	strip_quotes_token(char *token);
void	strip_quotes(t_list **tokens);

//Commands
int		is_debug(t_prog *term);
void	free_cmd(void *cmd);
void	free_pipeline(void *lstptr);
int		is_cmd_sep(t_list *token);
t_list	*split_commands(t_list *tokens);

//Command parse tree
t_ptree	*ptree_new(t_list *pipeline, int op);
void	push_ptree_stack(t_list	**stack, t_ptree *node);
t_ptree	*pop_ptree_stack(t_list	**stack);
t_ptree	*construct_parse_tree(t_list **ptree_list);
void	free_ptree_node(t_ptree *node, void *null);
void	print_ptree_node(t_ptree *ptree, void *null);
void	parse_pipeline(t_ptree *ptree, void *term);
t_ptree	*parse_line(char *line, t_prog *term);
void	traverse_ptree(t_ptree *ptree, int order, void (*f)(t_ptree *, void *),
			void *data);

//Redirection
int		is_redirect(t_list *token);
void	encode_redirect(t_list *token);
void	redirect_hdoc(t_cmd *cmd, t_list **rd_token,
			t_list *prev, t_prog *term);
void	redirect_out(t_cmd *cmd, t_list **rd_token, t_list *prev, int mode);
void	redirect_in(t_cmd *cmd, t_list **rd_token, t_list *prev);
void	apply_redirection(t_cmd *cmd, t_prog *term);
void	connect_pipes(t_list *pipeline);

#endif // PARSING_H
