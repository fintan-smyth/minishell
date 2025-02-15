/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:04:09 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/14 15:45:33 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	apply_quoting(char *quote, int *quoting, int mode)
// Sets the quoting mode and strips quote character from string.
{
	*quoting = mode;
	ft_memmove(quote, quote + 1, ft_strlen(quote));
}

void	strip_quotes_token(char *token)
// Strips unquoted quoting characters from a token.
{
	int	i;
	int	quoting;

	i = 0;
	quoting = 0;
	while (token[i])
	{
		if (token[i] == '\'' && quoting == Q_NONE)
			apply_quoting(&token[i], &quoting, Q_SINGLE);
		else if (token[i] == '\'' && quoting == Q_SINGLE)
			apply_quoting(&token[i], &quoting, Q_NONE);
		else if (token[i] == '\"' && quoting == Q_NONE)
			apply_quoting(&token[i], &quoting, Q_DOUBLE);
		else if (token[i] == '\"' && quoting == Q_DOUBLE)
			apply_quoting(&token[i], &quoting, Q_NONE);
		else
			i++;
	}
}

void	strip_excess_nodes(t_list **tokens)
// Strips any excess whitespace nodes generated by expansions.
{
	t_list	*current;
	t_list	*next;

	current = *tokens;
	while (ft_strwhitespace((char *)current->content))
	{
		*tokens = current->next;
		ft_lstdelone(current, free);
		current = *tokens;
	}
	while (current != NULL && current->next != NULL)
	{
		if (ft_strwhitespace((char *)current->next->content))
		{
			next = current->next->next;
			ft_lstdelone(current->next, free);
		}
		else if (is_cmd_sep(current->next))
		{
			ft_lstdelone(current->next, free);
			current->next = NULL;
			continue ;
		}
		current = current->next;
	}
}

void	strip_quotes(t_list **tokens)
// Strips unquoted quotes from a list of tokens.
{
	t_list	*current;

	strip_excess_nodes(tokens);
	current = *tokens;
	while (current != NULL)
	{
		if (*(char *)current->content == RD_HRD)
		{
			if (current->next == NULL)
				return ;
			current = current->next->next;
			continue ;
		}
		strip_quotes_token((char *)current->content);
		current = current->next;
	}
}
