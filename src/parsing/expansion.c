/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:45:57 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 01:56:48 by fsmyth           ###   ########.fr       */
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

void	apply_quoting(char *quote, int *quoting, int mode)
{
	*quoting = mode;
	ft_memmove(quote, quote + 1, ft_strlen(quote) + 1);
}

void	retokenise(t_list *token)
{
	t_list	*current;
	int		i;
	char	*line;

	i = 0;
	current = token;
	line = (char *)current->content;
	while (line[i])
	{
		if ()
	}
}

void	expand_token(char **token, t_term *term)
{
	char	*varp;
	int		i;
	int		quoting;

	i = 0;
	quoting = 0;
	while ((*token)[i])
	{
		if ((*token)[i] == '$' && quoting != SINGLE)
		{
			varp = &(*token)[i];
			expand_var_inplace(token, varp, term);
		}
		else if ((*token)[i] == '\'' && quoting == NONE)
			apply_quoting(&(*token)[i], &quoting, SINGLE);
		else if ((*token)[i] == '\'' && quoting == SINGLE)
			apply_quoting(&(*token)[i], &quoting, NONE);
		else if ((*token)[i] == '\"' && quoting == NONE)
			apply_quoting(&(*token)[i], &quoting, DOUBLE);
		else if ((*token)[i] == '\"' && quoting == DOUBLE)
			apply_quoting(&(*token)[i], &quoting, NONE);
		i++;
	}
}

void	expand_token_list(t_list *tokens, t_term *term)
{
	t_list	*current;

	current = tokens;
	while (current != NULL)
	{
		expand_token((char **)&current->content, term);
		current = current->next;
	}
}
