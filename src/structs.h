/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 17:41:34 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/06 16:20:31 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "../libft/libft.h"
# include <linux/limits.h>
# include <stdbool.h>

typedef struct s_env
{
	char	*name;
	char	*var;
}	t_env;

typedef struct s_entry
{
	char			d_name[NAME_MAX];
	unsigned char	d_type;
}	t_entry;

typedef struct s_ptree
{
	t_list			*pipeline;
	int				op;
	struct s_ptree	*left;
	struct s_ptree	*right;
}	t_ptree;

typedef struct s_term
{
	char	cwd[PATH_MAX];
	char	*prompt;
	char	*line;
	t_list	*env_list;
	int		status;
	int		parse_status;
	t_ptree	*ptree;
}	t_prog;

typedef struct s_cmd
{
	t_list	*tokens;
	char	**argv;
	int		argc;
	int		sep;
	int		condition;
	int		fd_in;
	int		fd_out;
	int		pipe[2];
	int		hdpipe[2];
	int		error;
	bool	rd_in;
	bool	rd_out;
}	t_cmd;

#endif
