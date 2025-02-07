/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:24:45 by myiu              #+#    #+#             */
/*   Updated: 2025/02/07 16:40:36 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parsing.h"

void	exit_shell(t_prog *term, t_cmd *cmd)
{
	int	status;

	status = term->status;
	if (cmd->argc > 2)
	{
		ft_putendl_fd("\e[32mminishell: \e[35mexit: \e[mtoo many arguments", 2);
		term->status = 1 << 8;
		return ;
	}
	else if (cmd->argc == 1)
		;
	else if (!ft_strnum(cmd->argv[1]))
	{
		status = 2 << 8;
		ft_putstr_fd("\e[32mminishell: \e[35mexit:\e[m ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putendl_fd(" numeric argument required", 2);
	}
	else
		status = ft_atoi(cmd->argv[1]) << 8;
	traverse_ptree(term->ptree, PST_ORD, free_ptree_node, NULL);
	cleanup(term);
	exit(WEXITSTATUS(status));
}
