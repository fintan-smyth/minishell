/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_vars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:57:45 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/03 20:27:30 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"
#include <stdlib.h>

int	valid_var_chr(char c)
// Checks if a char is valid to be used in an environment variable name
{
	if (ft_isalnum(c))
		return (1);
	if (c == '_')
		return (1);
	return (0);
}

char	*extend_line(char *line, char *extra)
// returns 'line' with 'extra' added to the end.
// If replaced, the old 'line' is freed
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

void	expand_exit_status(char **line, char *varp, t_prog *term)
{
	char	*status;
	char	*expanded;

	status = ft_itoa(WEXITSTATUS(term->status));
	*varp = 0;
	varp++;
	expanded = ft_strdup(*line);
	expanded = extend_line(expanded, status);
	expanded = extend_line(expanded, varp);
	free(status);
	free(*line);
	*line = expanded;
}

int	expand_var_inplace(char **line, char *varp, t_prog *term)
// Expands the environment variable at *varp in place.
// Returns 1 if variable name is valid so expansion is attempted.
// Returns 0 if no expansion attempted, leaving the '$' in place
{
	char	*expanded;
	char	*env_name;
	char	*env_var;
	int		i;

	*varp = 0;
	varp++;
	if (*varp == '?')
	{
		expand_exit_status(line, varp, term);
		return (1);
	}
	expanded = ft_strdup(*line);
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

void	expand_token_var(char **token, t_prog *term)
// Expands all environment variables in a token not blocked by quoting
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
