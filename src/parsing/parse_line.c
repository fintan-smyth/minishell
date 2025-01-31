/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 14:10:22 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/31 14:32:29 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "parsing.h"

t_list *parse_line(char *line, t_term *term)
{
	t_list	*tokens;
	t_list	*cmd_list;
	t_list	*current;

	tokens = tokenise(line);
	cmd_list = split_commands(tokens);
	current = cmd_list;
	while (current != NULL)
	{
		tokens = ((t_cmd *)current->content)->tokens;
		ft_printf("\e[1;33mTokenised\n\e[m");
		print_tokens(tokens);
		expand_token_list(tokens, term);
		ft_printf("\e[1;32m\nExpanded\n\e[m");
		print_tokens(tokens);
		strip_quotes(&tokens);
		ft_printf("\e[1;35m\nStripped\n\e[m");
		print_tokens(tokens);
		apply_redirection((t_cmd *)current->content);
		ft_printf("\e[1;35m\nRedirected\n\e[m");
		print_tokens(tokens);
		ft_printf("fd_out\t%d\nfd_in:\t%d\nerr:\t%d\n\n", ((t_cmd *)current->content)->fd_out, ((t_cmd *)current->content)->fd_in, ((t_cmd *)current->content)->error);
		((t_cmd *)current->content)->argv = (char **)lst_to_arr(tokens);
		((t_cmd *)current->content)->argc = count_args(((t_cmd *)current->content)->argv);
		current = current->next;
	}
	return (cmd_list);
}
