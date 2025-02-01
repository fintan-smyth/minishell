/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:00:13 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/01 15:07:14 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../minishell.h"
# include "../parsing/parsing.h"

void	echo(t_cmd *cmd);
void	cd(t_term *term, t_cmd *cmd);
void	pwd(t_term *term, t_cmd *cmd);
void	env(char **envp);
void	export_env(t_term *term, int argc, char **argv);
void	unset_env(t_term *term, int argc, char **argv);

#endif // BUILTINS_H
