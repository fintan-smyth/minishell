/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 17:19:48 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/02 18:10:46 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"
#include <fcntl.h>
#include <readline/readline.h>

int	is_redirect(t_list *token)
{
	char	*text;

	text = (char *)token->content;
	if (ft_strncmp(text, "<", 2) == 0)
		return (RD_IN);
	if (ft_strncmp(text, ">", 2) == 0)
		return (RD_OUT);
	if (ft_strncmp(text, ">>", 3) == 0)
		return (RD_APP);
	if (ft_strncmp(text, "<<", 3) == 0)
		return (RD_HRD);
	return (0);
}

void	encode_redirect(t_list *token)
{
	int		mode;

	mode = is_redirect(token);
	free(token->content);
	token->content = ft_strdup(" ");
	*(char *)token->content = mode;
}

void	apply_redirection(t_cmd *cmd, t_term *term)
{
	t_list	*current;
	t_list	*prev;
	int		mode;

	(void)term;
	current = cmd->tokens;
	prev = NULL;
	while (current != NULL)
	{
		mode = *(char *)current->content;
		if (mode == RD_IN)
			redirect_in(cmd, &current, prev);
		else if (mode == RD_OUT || mode == RD_APP)
			redirect_out(cmd, &current, prev, mode);
		else if (mode == RD_HRD)
			redirect_hdoc(cmd, &current, prev, term);
		if (current == NULL)
			current = prev->next;
		else
		{
			prev = current;
			current = current->next;
		}
	}
}
