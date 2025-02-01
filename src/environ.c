/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 12:45:55 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/01 14:26:02 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env(void *data)
{
	t_env	*env;
	env = (t_env *)data;
	free(env->name);
	free(env->var);
	free(env);
}

char	*construct_envp_line(char *name, char *env)
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

char	**construct_envp(t_list *env_list)
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

t_list	*getenv_node(t_list *env_list, char *name)
{
	t_list	*current;
	char	*env_name;

	current = env_list;
	while (current != NULL)
	{
		env_name = ((t_env *)current->content)->name;
		if (!ft_strncmp(env_name, name, ft_strlen(name) + 1))
			break ;
		current = current->next;
	}
	if (current == NULL)
		return (NULL);
	return (current);
}

char	*getenv_list(t_list *env_list, char *name)
{
	t_env	*env;
	t_list	*node;

	node = getenv_node(env_list, name);
	if (node == NULL)
		return (NULL);
	env = (t_env *)(node->content);
	if (env == NULL)
		return (NULL);
	return (env->var);
}

// void	env_list_del(t_list *env_list, char *name)
// {
//
// }

char	*get_shell(t_term *term, char *name)
{
	size_t	len;
	char	*shell;

	if (name[0] == '.' && name[1] == '/')
		name += 2;
	len = 2;
	len += ft_strlen(term->cwd);
	len += ft_strlen(name);
	shell = ft_calloc(len, 1);
	ft_strlcpy(shell, term->cwd, len);
	ft_strlcat(shell, "/", len);
	ft_strlcat(shell, name, len);
	return (shell);
}

void	env_list_add(t_list **lst, char *name, char *var)
{
	t_env	*env;

	env = ft_calloc(1, sizeof(*env));
	env->name = ft_strdup(name);
	if (var != NULL)
		env->var = ft_strdup(var);
	else
		env->var = NULL;
	ft_lstadd_back(lst, ft_lstnew(env));
}

void	init_env_list(t_term *term, char *name)
{
	char	*shell;

	env_list_add(&term->env_list, "HOME", getenv("HOME"));
	env_list_add(&term->env_list, "TERM", getenv("TERM"));
	env_list_add(&term->env_list, "USER", getenv("USER"));
	env_list_add(&term->env_list, "PATH", getenv("PATH"));
	env_list_add(&term->env_list, "TEST", getenv("TEST"));
	env_list_add(&term->env_list, "MS_DEBUG", getenv("MS_DEBUG"));
	env_list_add(&term->env_list, "PWD", term->cwd);
	env_list_add(&term->env_list, "OLDPWD", term->cwd);
	shell = get_shell(term, name);
	env_list_add(&term->env_list, "SHELL", shell);
	free(shell);
}
