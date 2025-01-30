/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:04:09 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 01:16:46 by fsmyth           ###   ########.fr       */
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
		if (token[i] == '\'' && quoting == NONE)
			apply_quoting(&token[i], &quoting, SINGLE);
		if (token[i] == '\'' && quoting == SINGLE)
			apply_quoting(&token[i], &quoting, NONE);
		if (token[i] == '\"' && quoting == NONE)
			apply_quoting(&token[i], &quoting, DOUBLE);
		if (token[i] == '\"' && quoting == DOUBLE)
			apply_quoting(&token[i], &quoting, NONE);
		i++;
	}
}

void	strip_quotes(t_list *tokens)
{
	t_list	*current;

	current = tokens;
	while (current != NULL)
	{
		strip_quotes_token((char *)current->content);
		current = current->next;
	}
}
