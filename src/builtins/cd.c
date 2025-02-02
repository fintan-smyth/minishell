/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:20:50 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/31 00:42:13 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parsing.h"

void	update_wd(t_term *term)
{
	t_env	*pwd;
	t_env	*oldpwd;

	pwd = (t_env *)getenv_node(term->env_list, "PWD")->content;
	oldpwd = (t_env *)getenv_node(term->env_list, "OLDPWD")->content;
	free(oldpwd->var);
	oldpwd->var = pwd->var;
	getcwd(term->cwd, PATH_MAX);
	pwd->var = ft_strdup(term->cwd);
}

static char	*get_path(t_list *env_list, char **argv, int *revert)
{
	char	*path;

	*revert = 0;
	if (argv[1] == NULL)
		path = getenv_list(env_list, "HOME");
	else if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		*revert = 1;
		path = getenv_list(env_list, "OLDPWD");
	}
	else
		path = argv[1];
	return (path);
}

void	cd(t_term *term, t_cmd *cmd)
{
	char	*path;
	int		revert;

	path = get_path(term->env_list, cmd->argv, &revert);
	if (cmd->argc > 2)
		ft_putendl_fd("\e[32mminishell: \e[35mcd: \e[mtoo many arguments", 2);
	else if (chdir(path) != 0)
	{
		ft_putstr_fd("\e[32mminishell: \e[35mcd: \e[m", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
	else
	{
		update_wd(term);
		if (revert == 1)
			ft_putendl_fd(term->cwd, cmd->fd_out);
	}
}
