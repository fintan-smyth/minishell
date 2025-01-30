/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:32:29 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 16:10:04 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"

typedef struct s_cmd
{
	t_list	*tokens;
	int		sep;
	int		fd_in;
	int		fd_out;
}	t_cmd;

enum
{
	END = 0,
	PIPE = 1,
	AND = 2,
	OR = 3,
};

enum
{
	NONE,
	SINGLE,
	DOUBLE,
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

#endif // PARSING_H
