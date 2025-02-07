/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:24:38 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/03 20:27:30 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env(char **envp)
// Executes the 'env' builtin command
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		ft_printf("%s\n", envp[i++]);
	free_split(&envp);
}

int	validate_export(char *arg, char **equals)
// Validates syntax for an 'export' command
{
	*equals = ft_strchr(arg, '=');
	if (*equals == NULL)
		return (0);
	if (*equals == arg)
		return (0);
	return (1);
}

void	export_env(t_prog *term, int argc, char **argv)
// Executes the 'export' builtin command
{
	int		i;

	if (argc < 2)
		env(construct_envp(term->env_list));
	else
	{
		i = 0;
		while (argv[++i] != NULL)
			envp_to_lst(term, argv[i]);
	}
}

void	unset_env(t_prog *term, int argc, char **argv)
// Executes the 'unset' builtin command
{
	t_list	*env_node;
	t_env	*env;
	int		i;

	if (argc < 2)
		return ;
	i = 1;
	while (argv[i] != NULL)
	{
		env_node = getenv_node(term->env_list, argv[i++]);
		if (env_node != NULL)
		{
			env = (t_env *)env_node->content;
			free(env->var);
			env->var = NULL;
		}
	}
}
