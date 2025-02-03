/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 17:36:43 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/02 17:37:20 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

int	check_in_home(char *path, char *home)
// Checks if the path is within the HOME directory
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
// Constructs a string to be used as the prompt for readline()
{
	size_t	prompt_size;
	char	*path;
	char	*status;

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
	if (term->status > 0)
	{
		// status = strerror(WEXITSTATUS(term->status));
		status = ft_itoa(WEXITSTATUS(term->status));
		ft_strlcat(term->prompt, " \e[m(\e[1;31m", prompt_size);
		ft_strlcat(term->prompt, status, prompt_size);
		ft_strlcat(term->prompt, "\e[m)", prompt_size);
		free(status);
	}
	ft_strlcat(term->prompt, "\e[1;32m > \e[m", prompt_size);
	return (term->prompt);
}
