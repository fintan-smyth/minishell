/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strip_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 01:04:09 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 01:16:46 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	strip_quotes(char **token)
{
	char	*stripped;
	int		i;

	i = 0;
	stripped = ft_strdup(*token);
}

// void	apply_quoting(char c, int *quoting)
// {
// 	if (c == '\'' && *quoting == NONE)
// 		*quoting = SINGLE;
// 	else if (c == '\'' && *quoting == SINGLE)
// 		*quoting = NONE;
// 	else if (c == '\"' && *quoting == NONE)
// 		*quoting = DOUBLE;
// 	else if (c == '\"' && *quoting == DOUBLE)
// 		*quoting = NONE;
// }
