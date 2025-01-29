/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:24:38 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/29 17:00:00 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		ft_printf("%s\n", envp[i++]);
	free_split(&envp);
}

int	validate_export(char *arg, char **equals)
{
	*equals = ft_strchr(arg, '=');
	if (*equals == NULL)
		return (0);
	if (*equals == arg)
		return (0);
	return (1);
}

void	export_env(t_term *term, int argc, char **argv)
{
	char	*equals;
	t_list	*existing;
	int		i;

	if (argc < 2)
		env(construct_envp(term->env_list));
	else
	{
		i = 0;
		while (argv[++i] != NULL)
		{
			if (!validate_export(argv[i], &equals))
				continue ;
			*equals = 0;
			existing = getenv_node(term->env_list, argv[i]);
			if (existing == NULL)
				env_list_add(&term->env_list, argv[i], equals + 1);
			else
			{
				free(((t_env *)existing->content)->var);
				((t_env *)existing->content)->var = ft_strdup(equals + 1);
			}
		}
	}
}

void	unset_env(t_term *term, int argc, char **argv)
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
