/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:24:44 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 19:04:52 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parsing.h"

void	pwd(t_term *term, t_cmd *cmd)
// Executes the 'pwd' builtin command
{
	ft_putendl_fd(term->cwd, cmd->fd_out);
}
