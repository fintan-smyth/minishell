/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:32:29 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 01:16:57 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../minishell.h"

typedef struct s_cmd
{
	t_list	*tokens;
	int		type;
}	t_cmd;

enum
{
	COMMAND = 0,
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
void	strip_quotes(t_list *tokens);

#endif // PARSING_H
