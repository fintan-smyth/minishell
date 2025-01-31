/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:50:15 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/31 00:23:44 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parsing/parsing.h"
#include <readline/history.h>
#include <readline/readline.h>

void	exec_cmd(t_term *term, t_cmd *cmd);

int	check_in_home(char *path, char *home)
{
	char	*terminated;
	size_t	size;

	if (home == NULL)
		return (0);
	size = ft_strlen(home) + 2;
	terminated = ft_calloc(size, 1);
	ft_strlcpy(terminated, home, size);
	if (ft_strncmp(path, terminated, size - 1) == 0)
		return (free(terminated), 1);
	ft_strlcat(terminated, "/", size);
	if (ft_strncmp(path, terminated, size - 1) == 0)
		return (free(terminated), 1);
	return (free(terminated), 0);
}

char	*get_prompt(t_term *term, char *home)
{
	size_t	prompt_size;
	char	*path;

	free(term->prompt);
	prompt_size = ft_strlen(term->cwd) + 200;
	term->prompt = ft_calloc(prompt_size, 1);
	ft_strlcat(term->prompt, "\n╭─\e[36m ", prompt_size);
	ft_strlcat(term->prompt, getenv_list(term->env_list, "USER"), prompt_size);
	ft_strlcat(term->prompt, "\e[31m@minishell\e[m:\e[1;34m ", prompt_size);
	if (check_in_home(term->cwd, home))
	{
		ft_strlcat(term->prompt, "~", prompt_size);
		path = ft_substr(term->cwd, ft_strlen(home), PATH_MAX);
		ft_strlcat(term->prompt, path, prompt_size);
		free(path);
	}
	else
		ft_strlcat(term->prompt, term->cwd, prompt_size);
	ft_strlcat(term->prompt, "\e[m\n╰─\e[1;32m> \e[m", prompt_size);
	return (term->prompt);
}

char	*get_prompt2(t_term *term, char *home)
{
	size_t	prompt_size;
	char	*path;

	free(term->prompt);
	prompt_size = ft_strlen(term->cwd) + 200;
	term->prompt = ft_calloc(prompt_size, 1);
	ft_strlcat(term->prompt, "\e[36m", prompt_size);
	ft_strlcat(term->prompt, getenv_list(term->env_list, "USER"), prompt_size);
	ft_strlcat(term->prompt, "\e[31m@minishell\e[m:\e[1;34m ", prompt_size);
	if (check_in_home(term->cwd, home))
	{
		ft_strlcat(term->prompt, "~", prompt_size);
		path = ft_substr(term->cwd, ft_strlen(home), PATH_MAX);
		ft_strlcat(term->prompt, path, prompt_size);
		free(path);
	}
	else
		ft_strlcat(term->prompt, term->cwd, prompt_size);
	ft_strlcat(term->prompt, "\e[1;32m > \e[m", prompt_size);
	return (term->prompt);
}

void	cleanup(t_term *term)
{
	ft_lstclear(&term->entries, free);
	ft_lstclear(&term->env_list, free_env);
	rl_clear_history();
	free(term->prompt);
	free(term);
}

int	main(int argc, char **argv)
{
	t_term	*term;
	t_list	*tokens;
	t_list	*cmd_list;
	t_list	*current_cmd;
	char	*line;

	(void)argc;
	term = ft_calloc(1, sizeof(*term));
	getcwd(term->cwd, PATH_MAX);
	init_env_list(term, argv[0]);
	get_entries(term);
	line = readline(get_prompt2(term, getenv_list(term->env_list, "HOME")));
	while (line != NULL)
	{
		if (*line == 0)
		{
			free(line);
			line = readline(get_prompt2(term, getenv_list(term->env_list, "HOME")));
			continue ;
		}
		tokens = tokenise(line);
		cmd_list = split_commands(tokens);
		current_cmd = cmd_list;
		while (current_cmd != NULL)
		{
			tokens = ((t_cmd *)current_cmd->content)->tokens;
			ft_printf("\e[1;33mTokenised\n\e[m");
			print_tokens(tokens);
			expand_token_list(tokens, term);
			ft_printf("\e[1;32m\nExpanded\n\e[m");
			print_tokens(tokens);
			strip_quotes(&tokens);
			ft_printf("\e[1;35m\nStripped\n\e[m");
			print_tokens(tokens);
			apply_redirection((t_cmd *)current_cmd->content);
			ft_printf("\e[1;35m\nRedirected\n\e[m");
			print_tokens(tokens);
			ft_printf("fd_out\t%d\nfd_in:\t%d\nerr:\t%d\n\n", ((t_cmd *)current_cmd->content)->fd_out, ((t_cmd *)current_cmd->content)->fd_in, ((t_cmd *)current_cmd->content)->error);
			((t_cmd *)current_cmd->content)->argv = (char **)lst_to_arr(tokens);
			((t_cmd *)current_cmd->content)->argc = count_args(((t_cmd *)current_cmd->content)->argv);
			exec_cmd(term, (t_cmd *)current_cmd->content);
			current_cmd = current_cmd->next;
		}
		add_history(line);
		free(line);
		ft_lstclear(&cmd_list, free_cmd);
		line = readline(get_prompt2(term, getenv_list(term->env_list, "HOME")));
	}
	cleanup(term);
}
