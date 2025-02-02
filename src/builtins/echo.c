/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:58:13 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/31 00:37:42 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing/parsing.h"

void	echo(t_cmd *cmd)
// Executes the 'echo' builtin command
{
	int		i;
	int		nl;

	if (cmd->argc < 2)
		write(1, "\n", 1);
	else
	{
		i = 0;
		nl = 1;
		if (ft_strncmp((cmd->argv)[1], "-n", 3) == 0)
		{
			nl = 0;
			i++;
		}
		while ((cmd->argv)[++i] != NULL)
		{
			if (i != 2 - nl)
				write(cmd->fd_out, " ", 1);
			ft_putstr_fd((cmd->argv)[i], cmd->fd_out);
		}
		if (nl == 1)
			write(cmd->fd_out, "\n", 1);
		else if (i != 2 && cmd->fd_out == 1)
			ft_putstr_fd("\e[7m%\e[m\n", 1);
	}
}
