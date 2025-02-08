/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:59:13 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/08 21:15:53 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	print_syntax_err(t_list *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token != NULL)
		ft_putstr_fd((char *)token->content, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putendl_fd("\'", 2);
}

int	handle_brackets_syntax(t_list *token, t_list *prev, int *brackets)
{
	if (*(char *)token->content == '(')
	{
		(*brackets)++;
		if (token_is_op(token->next) > 1)
			return (print_syntax_err(token), 0);
	}
	else if (*(char *)token->content == ')')
	{
		(*brackets)--;
		if (token_is_op(prev) > 1)
			return (print_syntax_err(token), 0);
	}
	if (*brackets < 0)
		return (print_syntax_err(token), 0);
	return (1);
}

int	check_op_syntax(t_list *tokens)
{
	t_list	*prev;
	int		brackets;

	brackets = 0;
	prev = NULL;
	if (ft_strchr("|&", *(char *)tokens->content)
		|| !handle_brackets_syntax(tokens, prev, &brackets))
		return (print_syntax_err(tokens), 0);
	while (tokens->next != NULL)
	{
		if (!handle_brackets_syntax(tokens->next, tokens, &brackets))
			return (0);
		if (ft_strchr("|&", *(char *)tokens->content)
			&& ft_strchr("|&", *(char *)tokens->next->content))
			return (print_syntax_err(tokens->next), 0);
		else if (ft_strchr("><", *(char *)tokens->content)
			&& ft_strchr("|&()<>", *(char *)tokens->next->content))
			return (print_syntax_err(tokens->next), 0);
		prev = tokens;
		tokens = tokens->next;
	}
	if (ft_strchr("|&><(", *(char *)tokens->content)
		|| brackets > 0)
		return (print_syntax_err(tokens->next), 0);
	return (1);
}

int	verify_tkn_syntax(t_list *tokens, t_prog *term)
{
	(void)term;
	if (!check_op_syntax(tokens))
		return (0);
	return (1);
}
