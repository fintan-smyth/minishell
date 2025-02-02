/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:04:09 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/01 13:08:05 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	apply_quoting(char *quote, int *quoting, int mode)
{
	*quoting = mode;
	ft_memmove(quote, quote + 1, ft_strlen(quote));
}

void	strip_quotes_token(char *token)
{
	int	i;
	int	quoting;

	i = 0;
	quoting = 0;
	while (token[i])
	{
		if (token[i] == '\'' && quoting == Q_NONE)
			apply_quoting(&token[i], &quoting, Q_SINGLE);
		if (token[i] == '\'' && quoting == Q_SINGLE)
			apply_quoting(&token[i], &quoting, Q_NONE);
		if (token[i] == '\"' && quoting == Q_NONE)
			apply_quoting(&token[i], &quoting, Q_DOUBLE);
		if (token[i] == '\"' && quoting == Q_DOUBLE)
			apply_quoting(&token[i], &quoting, Q_NONE);
		i++;
	}
}

void	strip_excess_nodes(t_list **tokens)
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
			current->next = next;
		}
		else if (is_cmd_sep(current->next))
		{
			ft_lstdelone(current->next, free);
			current->next = NULL;
		}
		current = current->next;
	}
}

void	strip_quotes(t_list **tokens)
{
	t_list	*current;

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
	strip_excess_nodes(tokens);
}
