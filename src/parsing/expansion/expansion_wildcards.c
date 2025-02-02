/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_wildcards.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:00:10 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/02 18:10:33 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

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

void	search_entries(char **expanded, t_term *term, char *linecpy)
{
	t_list	*entries;
	char	**d_names;
	int		i;

	entries = NULL;
	entries = get_entries(term);
	d_names = (char **)ft_lst_to_arr(entries);
	ft_qsort((void **)d_names, 0, ft_lstsize(entries) - 1,
		(int (*)(void *, void *))ft_strcmp);
	i = -1;
	while (d_names[++i] != NULL)
	{
		if (*d_names[i] == '.' && *linecpy != '.')
			;
		else if (ft_match_wc(d_names[i], linecpy, 5))
		{
			if (*expanded != NULL)
				*expanded = extend_line(*expanded, " ");
			*expanded = extend_line(*expanded, d_names[i]);
		}
	}
	free(d_names);
	ft_lstclear(&entries, free);
}

void	expand_wildcards(char **line, t_term *term)
{
	char	*linecpy;
	char	*expanded;

	expanded = NULL;
	linecpy = ft_strdup(*line);
	if (!encode_wildcards(linecpy))
		return (free(linecpy));
	strip_quotes_token(linecpy);
	search_entries(&expanded, term, linecpy);
	free(linecpy);
	if (expanded == NULL)
		return ;
	free(*line);
	*line = expanded;
}
