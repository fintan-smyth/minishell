/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 16:39:24 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/27 18:23:25 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <linux/limits.h>
#include <unistd.h>

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
	char	**paths;
	int		i;

	i = 0;
	paths = ft_split(getenv_list(term->env_list, "PATH"), ':');
	ft_bzero(cmd_path, PATH_MAX);
	ft_strlcat(cmd_path, cmd, PATH_MAX);
	while (access(cmd_path, X_OK))
	{
		ft_bzero(cmd_path, PATH_MAX);
		if (paths[i] == NULL)
			break ;
		ft_strlcpy(cmd_path, paths[i++], PATH_MAX);
		ft_strlcat(cmd_path, "/", PATH_MAX);
		ft_strlcat(cmd_path, cmd, PATH_MAX);
	}
	free_split(&paths);
	if (cmd_path[0] == 0)
		return (1);
	return (0);
}

int	handle_builtins(t_term *term, int argc, char **argv)
{
	if (!ft_strncmp(argv[0], "cd", 3))
		cd(term, argc, argv);
	else if (!ft_strncmp(argv[0], "pwd", 4))
		pwd(term);
	else if (!ft_strncmp(argv[0], "env", 4))
		env(construct_envp(term->env_list));
	else if (!ft_strncmp(argv[0], "export", 7))
		export_env(term, argc, argv);
	else if (!ft_strncmp(argv[0], "unset", 6))
		unset_env(term, argc, argv);
	else if (!ft_strncmp(argv[0], "echo", 5))
		echo(argc, argv);
	else
		return (0);
	return (1);
}

void	handle_args(t_term *term, int argc, char **argv)
{
	pid_t	child;
	int		status;
	char	cmd_path[PATH_MAX];

	if (argv[0] == NULL)
		return ;
	else if (!handle_builtins(term, argc, argv))
	{
		if (search_path(term, argv[0], cmd_path) == 0)
		{
			child = fork();
			if (child > 0)
				waitpid(child, &status, 0);
			else
			{
				execve(cmd_path, argv, construct_envp(term->env_list));
				exit(EXIT_FAILURE);
			}
		}
		else
			ft_printf("%s: command not found\n", argv[0]);
	}
}
