/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:45:57 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/29 20:24:35 by fsmyth           ###   ########.fr       */
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

void	expand_var_inplace(char **line, char *varp, t_term *term)
{
	char	*expanded;
	char	*env_name;
	char	*env_var;
	int		i;

	*varp = 0;
	expanded = NULL;
	expanded = ft_strdup(*line);
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
	free(*line);
	*line = expanded;
}

void	expand_token(t_list *token, t_term *term)
{
	char	*varp;
	int		i;
	int		quoting;

	i = 0;
	quoting = 0;
	while (((char *)token->content)[i])
	{
		if (((char *)token->content)[i] == '$' && quoting != SINGLE)
		{
			varp = &((char *)token->content)[i];
			expand_var_inplace((char **)&token->content, varp, term);
		}
		else if (((char *)token->content)[i] == '\'' && quoting == NONE)
			quoting = SINGLE;
		else if (((char *)token->content)[i] == '\'' && quoting == SINGLE)
			quoting = NONE;
		else if (((char *)token->content)[i] == '\"' && quoting == NONE)
			quoting = DOUBLE;
		else if (((char *)token->content)[i] == '\"' && quoting == DOUBLE)
			quoting = NONE;
		i++;
	}
}

void	expand_token_list(t_list *tokens, t_term *term)
{
	t_list	*current;

	current = tokens;
	while (current != NULL)
	{
		// while (ft_strchr((char *)current->content, '$'))
			expand_token(current, term);
		current = current->next;
	}
}
