/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:39:32 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/06 22:01:30 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <linux/limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <sys/wait.h>
# include <termios.h>
# include "structs.h"

//Cleanup
void	cleanup(t_prog *term);

//Entries
t_list	*get_entries(t_prog *term);
void	print_entries(t_list *entries);

//Input
char	*get_prompt(t_prog *term, char *home);

//Exec
int		count_args(char **args);
int		search_path(t_prog *term, char *cmd, char *cmd_path);
int		exec_cmd(t_prog *term, t_cmd *cmd);

//Env
void	free_env(void *env);
t_list	*getenv_node(t_list *env_list, char *name);
char	*getenv_list(t_list *env_list, char *name);
void	init_env_list(t_prog *term, char *name, char **env);
void	env_list_add(t_list **lst, char *name, char *var);
char	*construct_envp_line(char *name, char *env);
char	**construct_envp(t_list *list);
void	envp_to_lst(t_prog *term, char *envp);
void	env_change_or_add(t_prog *term, char *name, char *var);

//Signals
void	sig_handler(int sig);
void	setup_signals(void);
void	handle_eof(t_prog *term);
void	reset_child_sig(void);
#endif
