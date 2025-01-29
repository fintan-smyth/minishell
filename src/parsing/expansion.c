/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:45:57 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/29 19:51:38 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "../minishell.h"

int	valid_var_chr(char c)
{
	if (ft_isalnum(c))
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

char	*extend_line(char *line, char *extra)
{
	char	*out;

	if (line == NULL)
		return (extra);
	if (extra == NULL)
		return (line);
	out = ft_strjoin(line, extra);
	free(line);
	return (out);
}

void	expand_token(t_list *token, t_term *term)
{
	char	*varp;
	char	*env_name;
	char	*env_var;
	char	*expanded;
	int		i;

	if (*(char *)token->content == '\'')
		return ;
	varp = ft_strchr((char *)token->content, '$');
	if (varp == NULL)
		return ;
	*varp = 0;
	expanded = ft_strdup((char *)token->content);
	varp++;
	i = 0;
	while (valid_var_chr(varp[i]))
		i++;
	env_name = ft_strndup(varp, i);
	env_var = getenv_list(term->env_list, env_name);
	if (env_var != NULL)
		expanded = extend_line(expanded, env_var);
	expanded = extend_line(expanded, &varp[i]);
	free(env_name);
	free(token->content);
	token->content = expanded;
}

void	expand_token_list(t_list *tokens, t_term *term)
{
	t_list	*current;

	current = tokens;
	while (current != NULL)
	{
		while (ft_strchr((char *)current->content, '$'))
			expand_token(current, term);
		current = current->next;
	}
}
