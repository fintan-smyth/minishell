/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:38:03 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/31 14:49:11 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	connect_pipes(t_list *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;
	t_list	*lp;

	lp = cmd_list;
	while (lp->next != NULL)
	{
		current = (t_cmd *)lp->content;
		next = (t_cmd *)lp->next->content;
		if (current->sep == OP_PIPE)
		{
			pipe(next->pipe);
			if (current->fd_out == 1)
				current->fd_out = (next->pipe)[1];
			if (next->fd_in == 0)
				next->fd_in = (next->pipe)[0];
		}
		lp = lp->next;
	}
}
