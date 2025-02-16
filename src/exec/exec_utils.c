/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:46:00 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/14 18:37:36 by fsmyth           ###   ########.fr       */
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

static char	**init_search(t_prog *term, char *cmd,
						char *cmd_path, struct stat *statbuf)
{
	char		**paths;

	if (getenv_list(term->env_list, "PATH") == NULL)
		paths = ft_split("", ':');
	else
		paths = ft_split(getenv_list(term->env_list, "PATH"), ':');
	ft_bzero(cmd_path, PATH_MAX);
	if (ft_strchr(cmd, '/') != NULL)
		ft_strlcat(cmd_path, cmd, PATH_MAX);
	stat(cmd_path, statbuf);
	return (paths);
}

int	search_path(t_prog *term, char *cmd, char *cmd_path)
// Searches the PATH variable to see if an executable matches the cmd name.
// If found, places the full command path in the cmd_path array and returns 1
// If no executable is found, cmd_path is empty and return 0
{
	struct stat	*statbuf;
	char		**paths;
	int			i;

	statbuf = ft_calloc(1, sizeof(*statbuf));
	paths = init_search(term, cmd, cmd_path, statbuf);
	i = 0;
	while (access(cmd_path, X_OK) || !(statbuf->st_mode & S_IFREG))
	{
		ft_bzero(cmd_path, PATH_MAX);
		if (paths[i] == NULL || ft_strchr(cmd, '/') != NULL)
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

void	handle_exit_sig(t_prog *term)
{
	if (WIFSIGNALED(term->status))
	{
		if (WTERMSIG(term->status) == SIGQUIT)
		{
			if (WCOREDUMP(term->status))
				ft_putendl_fd("Quit (core dumped)", 2);
			else
				ft_putendl_fd("Quit", 2);
		}
		else if (WTERMSIG(term->status) == SIGINT)
			ft_putendl_fd("", 2);
		term->status += ((128 + WTERMSIG(term->status)) << 8);
	}
}
