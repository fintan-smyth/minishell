/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:35:24 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/06 16:37:53 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

void	delimit_token(t_list **tokens, char **lp, int *i)
// Adds a new node to the list of tokens, containing *i characters of
// text starting at *lp.
// Moves *lp forward and resets *i.
{
	t_list	*new_token;

	new_token = ft_lstnew(ft_strndup(*lp, *i));
	ft_lstadd_back(tokens, new_token);
	*lp += *i;
	*i = -1;
}

int	apply_quoting_tokenise(int *quoting, char c, char *q_char)
// Toggles quoting rules when tokenising a line.
{
	int	out;

	out = 0;
	if (*quoting)
	{
		out = 1;
		if (c == *q_char)
			*quoting = 0;
	}
	else if (c == '\'' || c == '\"')
	{
		out = 1;
		*quoting = 1;
		*q_char = c;
	}
	return (out);
}

int	handle_ops_tokenise(t_list **tokens, char **lp, int *i)
// Handles operators when tokenising a line.
{
	int	out;

	out = 0;
	if (*i != 0 && is_op((*lp)[*i - 1]))
	{
		out = 1;
		if (!matches_prev_op((*lp)[*i], (*lp)[*i - 1]))
			delimit_token(tokens, lp, i);
	}
	else if (is_op((*lp)[*i]) && *i != 0)
	{
		out = 1;
		delimit_token(tokens, lp, i);
	}
	return (out);
}

int	handle_space_tokenise(t_list **tokens, char **line, char **lp, int *i)
// Handles whitespace when tokenising a line.
{
	int	out;

	out = 0;
	if (ft_iswhitespace((*lp)[*i]))
	{
		out = 1;
		if (&(*lp)[*i] != *line && *i != 0 && !ft_iswhitespace((*lp)[*i - 1]))
			delimit_token(tokens, lp, i);
		else
		{
			(*lp) += *i + 1;
			*i = -1;
		}
	}
	return (out);
}

t_list	*tokenise(char *line)
// Splits a line into tokens according to posix shell syntax rules.
// Returns a list of tokens.
{
	t_list	*tokens;
	char	*lp;
	int		quoting;
	char	q_char;
	int		i;

	lp = line;
	i = -1;
	quoting = 0;
	q_char = 0;
	tokens = NULL;
	while (lp[++i])
	{
		if (apply_quoting_tokenise(&quoting, lp[i], &q_char))
			;
		else if (handle_ops_tokenise(&tokens, &lp, &i))
			;
		else
			handle_space_tokenise(&tokens, &line, &lp, &i);
	}
	if (i > 0)
		delimit_token(&tokens, &lp, &i);
	return (tokens);
}
