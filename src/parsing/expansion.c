/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 18:45:57 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/01 21:35:46 by fsmyth           ###   ########.fr       */
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

int	encode_wildcards(char *token)
{
	int		i;
	int		wild;
	int		quoting;

	i = 0;
	quoting = 0;
	wild = 0;
	while (token[i])
	{
		if (token[i] == '*' && quoting == Q_NONE)
		{
			token[i] = 5;
			wild = 1;
		}
		else if (token[i] == '\'' && quoting == Q_NONE)
			quoting = Q_SINGLE;
		else if (token[i] == '\'' && quoting == Q_SINGLE)
			quoting = Q_NONE;
		else if (token[i] == '\"' && quoting == Q_NONE)
			quoting = Q_DOUBLE;
		else if (token[i] == '\"' && quoting == Q_DOUBLE)
			quoting = Q_NONE;
		i++;
	}
	return (wild);
}

void	expand_wildcards(char **line, t_term *term)
{
	t_list	*entry;
	char	*linecpy;
	char	*d_name;
	char	*expanded;

	expanded = NULL;
	linecpy = ft_strdup(*line);
	if (!encode_wildcards(linecpy))
		return (free(linecpy));
	strip_quotes_token(linecpy);
	entry = term->entries;
	while (entry != NULL)
	{
		d_name = ((t_entry *)entry->content)->d_name;
		if (*d_name == '.' && *linecpy != '.')
			;
		else if (ft_match_wc(d_name, linecpy, 5))
		{
			if (expanded != NULL)
				expanded = extend_line(expanded, " ");
			expanded = extend_line(expanded, d_name);
		}
		entry = entry->next;
	}
	free(linecpy);
	if (expanded == NULL)
		return ;
	free(*line);
	*line = expanded;
}

void	delimit_retoken(t_list **next, char *start)
{
	t_list	*new;

	new = ft_lstnew(ft_strdup(start));
	ft_lstadd_front(next, new);
}

void	retokenise(t_list *token)
{
	t_list	*current;
	int		i;
	char	*line;
	int		quoting;
	char	q_char;

	current = token;
	line = (char *)current->content;
	i = ft_strlen(line) - 1;
	quoting = 0;
	q_char = 0;
	while (i >= 0)
	{
		if (quoting)
		{
			if (line[i] == q_char)
				quoting = 0;
		}
		else if (line[i] == ' ')
		{
			line[i] = 0;
			if (line[i + 1] != 0)
				delimit_retoken(&token->next, &line[i + 1]);
		}
		else if (line[i] == '\'' || line[i] == '\"')
		{
			quoting = 1;
			q_char = line[i];
		}
		i--;
	}
}

void	expand_token_list(t_list *tokens, t_term *term)
{
	t_list	*current;

	current = tokens;
	while (current != NULL)
	{
		if (*(char *)current->content == RD_HRD)
		{
			if (current->next == NULL)
				return ;
			current = current->next->next;
			continue ;
		}
		expand_token_var((char **)&current->content, term);
		retokenise(current);
		expand_wildcards((char **)&current->content, term);
		retokenise(current);
		current = current->next;
	}
}
