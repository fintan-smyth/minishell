/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:35:24 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/01 17:50:29 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	**lst_to_arr(t_list *list)
{
	void	**array;
	t_list	*current;
	int		i;

	array = ft_calloc(ft_lstsize(list) + 1, sizeof(void *));
	i = 0;
	current = list;
	while (current != NULL)
	{
		array[i++] = current->content;
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}

int	is_op(char c)
{
	if (ft_strchr("><|&", c) == NULL)
		return (0);
	return (1);
}

int	matches_prev_op(char c, char prev)
{
	if (prev == '<' && c == '<')
		return (1);
	else if (prev == '>' && c == '>')
		return (1);
	else if (prev == '|' && c == '|')
		return (1);
	else if (prev == '&' && c == '&')
		return (1);
	return (0);
}

void	delimit_token(t_list **tokens, char **lp, int *i)
{
	t_list	*new_token;

	new_token = ft_lstnew(ft_strndup(*lp, *i));
	ft_lstadd_back(tokens, new_token);
	*lp += *i;
	*i = -1;
}

t_list	*tokenise(char *line)
{
	char	*lp;
	t_list	*tokens;
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
		// if (ft_lstsize(tokens) == 2)
		// 	ft_lstadd_front(&tokens->next, ft_lstnew(ft_strdup("ADDED FRONT")));
		// if (tokens != NULL)
		// {
		// 	ft_printf("<%s>\t<%s>\t<%5d>\n", lp, (char *)(ft_lstlast(tokens)->content), ft_lstsize(tokens));
		// 	usleep(200000);
		// }
		if (quoting)
		{
			if (lp[i] == q_char)
				quoting = 0;
		}
		else if (i != 0 && is_op(lp[i - 1]))
		{
			if (!matches_prev_op(lp[i], lp[i - 1]))
				delimit_token(&tokens, &lp, &i);
		}
		else if (lp[i] == '\'' || lp[i] == '\"')
		{
			quoting = 1;
			q_char = lp[i];
		}
		else if (is_op(lp[i]) && i != 0)
			delimit_token(&tokens, &lp, &i);
		else if (lp[i] == ' ')
		{
			if (&lp[i] != line && i != 0 &&lp[i - 1] != ' ')
				delimit_token(&tokens, &lp, &i);
			else
			{
				lp += i + 1;
				i = -1;
			}
		}
	}
	if (i > 0)
		delimit_token(&tokens, &lp, &i);
	return (tokens);
}

void	print_tokens(t_list *tokens)
{
	t_list	*current;

	current = tokens;
	while (current != NULL)
	{
		if (*(char *)current->content == RD_IN)
			ft_printf("token:\t\e[7;35mRD_IN  <\e[m\n");
		else if (*(char *)current->content == RD_OUT)
			ft_printf("token:\t\e[7;32mRD_OUT >\e[m\n");
		else if (*(char *)current->content == RD_APP)
			ft_printf("token:\t\e[7;36mRD_APP >>\e[m\n");
		else if (*(char *)current->content == RD_HRD)
			ft_printf("token:\t\e[7;31mRD_HRD <<\e[m\n");
		else
			ft_printf("token:\t<%s>\n", (char *)current->content);
		current = current->next;
	}
}
