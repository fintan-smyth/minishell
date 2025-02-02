/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:46:00 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/02 17:46:27 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
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
	struct stat	*statbuf;
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
