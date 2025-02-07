/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:24:44 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/07 17:36:25 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parsing.h"

void	pwd(t_prog *term, t_cmd *cmd)
// Executes the 'pwd' builtin command
{
	ft_putendl_fd(term->cwd, cmd->fd_out);
	term->status = 0;
}
