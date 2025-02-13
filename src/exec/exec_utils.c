/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:46:00 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/03 20:27:30 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/stat.h>

int	count_args(char **argv)
// Counts the number of args in an **argv.
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	return (i);
}

int	search_path(t_prog *term, char *cmd, char *cmd_path)
// Searches the PATH variable to see if an executable matches the cmd name.
// If found, places the full command path in the cmd_path array and returns 1
// If no executable is found, cmd_path is empty and return 0
{
	struct stat	*statbuf;
	char		**paths;
	int			i;

	i = 0;
	statbuf = ft_calloc(1, sizeof(*statbuf));
	if (getenv_list(term->env_list, "PATH") == NULL)
		paths = ft_split("", ':');
	else
		paths = ft_split(getenv_list(term->env_list, "PATH"), ':');
	ft_bzero(cmd_path, PATH_MAX);
	if (ft_strchr(cmd, '/') != NULL)
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
		return (0);
	return (1);
}
