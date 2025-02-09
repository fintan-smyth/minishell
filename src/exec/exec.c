/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:39:24 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/09 04:26:09 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../builtins/builtins.h"

int	handle_builtins(t_prog *term, t_cmd *cmd)
// Checks if a given command is a builtin, and if so executes it.
// Returns 1 if command was a builtin
// Returns 0 if command was not a builtin
{
	if (!ft_strncmp((cmd->argv)[0], "cd", 3))
		cd(term, cmd);
	else if (!ft_strncmp((cmd->argv)[0], "pwd", 4))
		pwd(term, cmd);
	else if (!ft_strncmp((cmd->argv)[0], "env", 4))
		env(term, cmd, construct_envp(term->env_list));
	else if (!ft_strncmp((cmd->argv)[0], "export", 7))
		export_env(term, cmd);
	else if (!ft_strncmp((cmd->argv)[0], "unset", 6))
		unset_env(term, cmd);
	else if (!ft_strncmp((cmd->argv)[0], "echo", 5))
		echo(term, cmd);
	else if (!ft_strncmp((cmd->argv)[0], "exit", 5))
		exit_shell(term, cmd);
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
		close((cmd->pipe)[1]);
		close((cmd->pipe)[0]);
		cmd->pipe[0] = -1;
		cmd->pipe[1] = -1;
	}
	signal(SIGINT, SIG_IGN);
	waitpid(child, status, 0);
	setup_signals();
}

int	ft_lst_exists(t_list *lst, void *match, int (*cmp)(void *, void *))
{
	t_list	*current;

	current = lst;
	while (current != NULL)
	{
		if (cmp(current->content, match) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

int	int_cmp(void *num1, void *num2)
{
	int	diff;
	int	nbr1;
	int	nbr2;

	nbr1 = *(int *)num1;
	nbr2 = *(int *)num2;
	diff = nbr1 - nbr2;
	return (diff);
}

void	close_fds_child(t_list *pipeline)
{
	t_list	*current;
	t_cmd	*cmd;
	t_list	*fds;

	fds = NULL;
	current = pipeline;
	while (current != NULL)
	{
		cmd = (t_cmd *)current->content;
		if (cmd->fd_in > 2
			&& !ft_lst_exists(fds, &cmd->fd_in, int_cmp))
			ft_lstadd_back(&fds, ft_lstnew(&cmd->fd_in));
		if (cmd->fd_out > 2
			&& !ft_lst_exists(fds, &cmd->fd_out, int_cmp))
			ft_lstadd_back(&fds, ft_lstnew(&cmd->fd_out));
		if (cmd->pipe[0] > 2
			&& !ft_lst_exists(fds, &cmd->pipe[0], int_cmp))
			ft_lstadd_back(&fds, ft_lstnew(&cmd->pipe[0]));
		if (cmd->pipe[1] > 2
			&& !ft_lst_exists(fds, &cmd->pipe[1], int_cmp))
			ft_lstadd_back(&fds, ft_lstnew(&cmd->pipe[1]));
		current = current->next;
	}
	current = fds;
	while (current != NULL)
	{
		close(*(int *)current->content);
		current = current->next;
	}
	ft_lstclear(&fds, NULL);
}

void	handle_child(t_cmd *cmd, t_prog *term, t_list *pipeline, char *cmd_path)
// Branch of commands to be executed by child after fork.
// dups the appropriate file descriptors and executes the passed in command.
{
	(void)pipeline;
	dup2(cmd->fd_in, 0);
	dup2(cmd->fd_out, 1);
	close_fds_child(pipeline);
	reset_child_sig();
	execve(cmd_path, cmd->argv, construct_envp(term->env_list));
	exit(EXIT_FAILURE);
}

pid_t	exec_cmd(t_prog *term, t_cmd *cmd, t_list *pipeline)
// Executes the passed in command.
// Returns the exit status of command.
{
	pid_t	child;
	char	cmd_path[PATH_MAX];

	// term->status = 0;
	child = -1;
	if ((cmd->argv)[0] == NULL)
		return (0);
	if (!handle_builtins(term, cmd))
	{
		if (search_path(term, (cmd->argv)[0], cmd_path) != 0)
		{
			child = fork();
			if (child == 0)
				handle_child(cmd, term, pipeline, cmd_path);
		}
		else
		{
			term->status = 127 << 8;
			ft_putstr_fd(cmd->argv[0], 2);
			ft_putendl_fd(": command not found", 2);
		}
	}
	return (child);
}

void	execute_pipeline_alt(t_list *pipeline, t_prog *term)
{
	t_list	*current;
	t_cmd	*cmd;
	pid_t	*child;
	t_list	*pids;
	int		status;

	pids = NULL;
	current = pipeline;
	while (current != NULL)
	{
		cmd = (t_cmd *)current->content;
		child = ft_calloc(1, sizeof(pid_t));
		*child = exec_cmd(term, cmd, current);
		free_cmd(cmd);
		if (*child == -1)
			status = term->status;
		ft_lstadd_back(&pids, ft_lstnew(child));
		current = current->next;
	}
	current = pids;
	signal(SIGINT, SIG_IGN);
	while (current != NULL)
	{
		if (*(pid_t *)current->content != -1)
			waitpid(*(pid_t *)current->content, &term->status, 0);
		else
			term->status = status;
		current = current->next;
	}
	setup_signals();
	ft_lstclear(&pids, free);
}
