/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:20:50 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/27 16:41:15 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	cd(t_term *term, int argc, char **args)
{
	char	*path;
	int		revert;

	revert = 0;
	if (args[1] == NULL)
		path = getenv_list(term->env_list, "HOME");
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		revert = 1;
		path = getenv_list(term->env_list, "OLDPWD");
	}
	else
		path = args[1];
	if (argc > 2)
		ft_printf("\e[32mminishell: \e[35mcd: \e[mtoo many arguments\n", path);
	else if (chdir(path) != 0)
		ft_printf("\e[32mminishell: \e[35mcd: \e[m%s", strerror(errno));
	else
	{
		update_wd(term);
		if (revert == 1)
			ft_printf("%s\n", term->cwd);
		get_entries(term);
	}
}
