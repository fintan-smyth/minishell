/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 19:00:13 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/30 19:05:05 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BULTINS_H
# define BULTINS_H

# include "../minishell.h"
# include "../parsing/parsing.h"

void	echo(int argc, char **argv);
void	cd(t_term *term, int argc, char **args);
void	pwd(t_term *term, t_cmd *cmd);
void	env(char **envp);
void	export_env(t_term *term, int argc, char **argv);
void	unset_env(t_term *term, int argc, char **argv);

#endif // BUILTINS_H
