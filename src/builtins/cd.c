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

void	cd(t_term *term, t_cmd *cmd)
{
	char	*path;
	int		revert;

	revert = 0;
	if ((cmd->argv)[1] == NULL)
		path = getenv_list(term->env_list, "HOME");
	else if (ft_strncmp((cmd->argv)[1], "-", 2) == 0)
	{
		revert = 1;
		path = getenv_list(term->env_list, "OLDPWD");
	}
	else
		path = (cmd->argv)[1];
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
		get_entries(term);
	}
}
