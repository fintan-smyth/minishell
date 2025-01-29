/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:32:29 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/29 20:19:46 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "../../libft/libft.h"

typedef struct s_cmd
{
	t_list	*tokens;
	int		type;
}	t_cmd;

enum
{
	COMMAND = 0,
	PIPE = 1,
	AND = 2,
	OR = 3,
};

enum
{
	NONE,
	SINGLE,
	DOUBLE,
};

#endif // PARSING_H
