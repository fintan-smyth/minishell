/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:57:45 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/02 18:10:21 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

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

	if (line == NULL && extra == NULL)
		return (NULL);
	if (line == NULL)
		return (ft_strdup(extra));
	if (extra == NULL)
		return (line);
	out = ft_strjoin(line, extra);
	free(line);
	return (out);
}

int	expand_var_inplace(char **line, char *varp, t_term *term)
{
	char	*expanded;
	char	*env_name;
	char	*env_var;
	int		i;

	*varp = 0;
	expanded = ft_strdup(*line);
	varp++;
	i = 0;
	while (valid_var_chr(varp[i]))
		i++;
	if (i == 0)
	{
		free(expanded);
		return (0);
	}
	env_name = ft_strndup(varp, i);
	env_var = getenv_list(term->env_list, env_name);
	if (env_var != NULL)
		expanded = extend_line(expanded, env_var);
	expanded = extend_line(expanded, &varp[i]);
	free(env_name);
	free(*line);
	*line = expanded;
	return (1);
}

void	expand_token_var(char **token, t_term *term)
{
	char	*varp;
	int		i;
	int		quoting;

	i = 0;
	quoting = 0;
	while ((*token)[i])
	{
		if ((*token)[i] == '$' && quoting != Q_SINGLE)
		{
			varp = &(*token)[i];
			if (!expand_var_inplace(token, varp, term))
				*varp = '$';
		}
		else if ((*token)[i] == '\'' && quoting == Q_NONE)
			quoting = Q_SINGLE;
		else if ((*token)[i] == '\'' && quoting == Q_SINGLE)
			quoting = Q_NONE;
		else if ((*token)[i] == '\"' && quoting == Q_NONE)
			quoting = Q_DOUBLE;
		else if ((*token)[i] == '\"' && quoting == Q_DOUBLE)
			quoting = Q_NONE;
		i++;
	}
}
