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
// Loops over a list of pipelines. For each pieline loops over 
// each command and connects any necessary pipes.
{
	t_cmd	*current;
	t_cmd	*next;
	t_list	*current_cmd;
	t_list	*current_pipeline;

	current_pipeline = cmd_list;
	while (current_pipeline != NULL)
	{
		current_cmd = (t_list *)current_pipeline->content;
		while (current_cmd->next != NULL)
		{
			current = (t_cmd *)current_cmd->content;
			next = (t_cmd *)current_cmd->next->content;
			if (current->sep == OP_PIPE)
			{
				pipe(next->pipe);
				if (current->fd_out == 1)
					current->fd_out = (next->pipe)[1];
				if (next->fd_in == 0)
					next->fd_in = (next->pipe)[0];
			}
			current_cmd = current_cmd->next;
		}
		current_pipeline = current_pipeline->next;
	}
}
