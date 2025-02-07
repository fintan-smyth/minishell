/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:43:31 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/07 17:34:34 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	validate_export(char *arg, char **equals);

void	free_env(void *data)
// Frees a t_env struct
{
	t_env	*env;

	env = (t_env *)data;
	free(env->name);
	free(env->var);
	free(env);
}

char	*construct_envp_line(char *name, char *env)
// Constructs a line to be used in the **envp array
{
	size_t	len;
	char	*line;

	len = ft_strlen(name);
	len += ft_strlen(env);
	line = ft_calloc(len + 2, 1);
	ft_strlcpy(line, name, len + 2);
	ft_strlcat(line, "=", len + 2);
	ft_strlcat(line, env, len + 2);
	return (line);
}

void	env_change_or_add(t_prog *term, char *name, char *var)
{
	t_list	*existing;

	existing = getenv_node(term->env_list, name);
	if (existing == NULL)
		env_list_add(&term->env_list, name, var);
	else
	{
		free(((t_env *)existing->content)->var);
		((t_env *)existing->content)->var = ft_strdup(var);
	}
}

void	envp_to_lst(t_prog *term, char *envp)
{
	char	*equals;

	if (!validate_export(envp, &equals))
	{
		term->status = 1 << 8;
		return ;
	}
	*equals = 0;
	env_change_or_add(term, envp, equals + 1);
	term->status = 0;
}

char	**construct_envp(t_list *env_list)
// Constructs the **envp array to pe passed to execve() calls
{
	char	**envp;
	t_list	*current;
	t_env	*env;
	int		i;

	current = env_list;
	envp = ft_calloc(ft_lstsize(env_list) + 1, sizeof(char *));
	i = 0;
	while (current != NULL)
	{
		env = (t_env *)current->content;
		if (env->var != NULL)
			envp[i++] = construct_envp_line(env->name, env->var);
		current = current->next;
	}
	envp[i] = NULL;
	return (envp);
}
