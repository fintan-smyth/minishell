/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:39:24 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/31 00:40:54 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parsing.h"
#include "builtins/builtins.h"
#include <sys/stat.h>

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	return (i);
}

int	search_path(t_term *term, char *cmd, char *cmd_path)
{
	struct stat *statbuf;
	char		**paths;
	int			i;

	i = 0;
	statbuf = malloc(sizeof(*statbuf));
	paths = ft_split(getenv_list(term->env_list, "PATH"), ':');
	ft_bzero(cmd_path, PATH_MAX);
	ft_strlcat(cmd_path, cmd, PATH_MAX);
	stat(cmd_path, statbuf);
	while (access(cmd_path, X_OK) || !(statbuf->st_mode & S_IFREG))
	{
		ft_bzero(cmd_path, PATH_MAX);
		if (paths[i] == NULL)
			break ;
		ft_strlcpy(cmd_path, paths[i++], PATH_MAX);
		ft_strlcat(cmd_path, "/", PATH_MAX);
		ft_strlcat(cmd_path, cmd, PATH_MAX);
		stat(cmd_path, statbuf);
	}
	free_split(&paths);
	free(statbuf);
	if (cmd_path[0] == 0)
		return (1);
	return (0);
}

int	handle_builtins(t_term *term, t_cmd *cmd)
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

void	exec_cmd(t_term *term, t_cmd *cmd)
{
	pid_t	child;
	int		status;
	char	cmd_path[PATH_MAX];

	if ((cmd->argv)[0] == NULL)
		return ;
	else if (!handle_builtins(term, cmd))
	{
		if (search_path(term, (cmd->argv)[0], cmd_path) == 0)
		{
			child = fork();
			if (child > 0)
				waitpid(child, &status, 0);
			else
			{
				dup2(cmd->fd_in, 0);
				dup2(cmd->fd_out, 1);
				execve(cmd_path, cmd->argv, construct_envp(term->env_list));
				exit(EXIT_FAILURE);
			}
		}
		else
			ft_printf("%s: command not found\n", (cmd->argv)[0]);
	}
}
