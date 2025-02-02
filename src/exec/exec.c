/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:39:24 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/02 17:48:17 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parsing.h"
#include "../builtins/builtins.h"

int	handle_builtins(t_term *term, t_cmd *cmd)
// Checks if a given command is a builtin, and if so executes it.
// Returns 1 if command was a builtin
// Returns 0 if command was not a builtin
{
	if (!ft_strncmp((cmd->argv)[0], "cd", 3))
		cd(term, cmd);
	else if (!ft_strncmp((cmd->argv)[0], "pwd", 4))
		pwd(term, cmd);
	else if (!ft_strncmp((cmd->argv)[0], "env", 4))
		env(construct_envp(term->env_list));
	else if (!ft_strncmp((cmd->argv)[0], "export", 7))
		export_env(term, cmd->argc, cmd->argv);
	else if (!ft_strncmp((cmd->argv)[0], "unset", 6))
		unset_env(term, cmd->argc, cmd->argv);
	else if (!ft_strncmp((cmd->argv)[0], "echo", 5))
		echo(cmd);
	else
		return (0);
	return (1);
}

void	handle_parent(t_cmd *cmd, pid_t child, int *status)
// Branch of commands to be executed by parent after fork.
// Cleans up fds, waits for child to exit, and collects exit status.
{
	if ((cmd->pipe)[0] > 0)
	{
		close((cmd->pipe)[0]);
		close((cmd->pipe)[1]);
		if (cmd->fd_in == (cmd->pipe)[0])
			cmd->fd_in = 0;
	}
	if (cmd->sep == OP_PIPE)
		cmd->fd_out = 1;
	waitpid(child, status, 0);
}

void	handle_child(t_cmd *cmd, t_term *term, char *cmd_path)
// Branch of commands to be executed by child after fork.
// dups the appropriate file descriptors and executes the passed in command.
{
	if (cmd->fd_in == (cmd->pipe)[0])
		close((cmd->pipe)[1]);
	dup2(cmd->fd_in, 0);
	dup2(cmd->fd_out, 1);
	execve(cmd_path, cmd->argv, construct_envp(term->env_list));
	exit(EXIT_FAILURE);
}

void	exec_cmd(t_term *term, t_cmd *cmd)
// Executes the passed in command.
{
	pid_t	child;
	int		status;
	char	cmd_path[PATH_MAX];

	if ((cmd->argv)[0] == NULL)
		return ;
	else if (!handle_builtins(term, cmd))
	{
		if (search_path(term, (cmd->argv)[0], cmd_path) != 0)
		{
			child = fork();
			if (child > 0)
				handle_parent(cmd, child, &status);
			else
				handle_child(cmd, term, cmd_path);
		}
		else
		{
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putendl_fd(": command not found", 2);
		}
	}
}
