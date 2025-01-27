/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:39:32 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/27 18:31:48 by fsmyth           ###   ########.fr       */
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

typedef struct s_term
{
	char	cwd[PATH_MAX];
	char	*prompt;
	t_list	*env_list;
	t_list	*entries;
}	t_term;

//Cleanup
void	cleanup(t_term *term);

//Entries
void	get_entries(t_term *term);
void	print_entries(t_term *term);

//Builtins
void	echo(int argc, char **argv);
void	cd(t_term *term, int argc, char **args);
void	pwd(t_term *term);
void	env(char **envp);
void	export_env(t_term *term, int argc, char **argv);
void	unset_env(t_term *term, int argc, char **argv);

//Args
int		count_args(char **args);
void	handle_args(t_term *term, int argc, char **argv);

//Env
void	free_env(void *env);
t_list	*getenv_node(t_list *env_list, char *name);
char	*getenv_list(t_list *env_list, char *name);
void	init_env_list(t_term *term, char *name);
void	env_list_add(t_list **lst, char *name, char *var);
char	*construct_envp_line(char *name, char *env);
char	**construct_envp(t_list *env_list);

#endif
