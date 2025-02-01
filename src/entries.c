/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entries.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:18:41 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/27 16:41:28 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <dirent.h>

t_entry	*copy_dirent(struct dirent *dirent)
{
	t_entry	*entry;

	entry = ft_calloc(1, sizeof(*entry));
	ft_strlcpy(entry->d_name, dirent->d_name, NAME_MAX);
	entry->d_type = dirent->d_type;
	return (entry);
}

void	get_entries(t_term *term)
{
	DIR				*dir;
	struct dirent	*dirent;
	t_entry			*entry;

	dir = opendir(term->cwd);
	ft_lstclear(&term->entries, free);
	dirent = readdir(dir);
	while (dirent != NULL)
	{
		if (ft_strncmp(dirent->d_name, ".", 2) && ft_strncmp(dirent->d_name, "..", 3))
		{
			entry = copy_dirent(dirent);
			ft_lstadd_front(&term->entries, ft_lstnew(entry));
		}
		dirent = readdir(dir);
	}
	closedir(dir);
}

char	*get_dname(t_list *node)
{
	return (((t_entry *)node->content)->d_name);
}

unsigned char	get_dtype(t_list *node)
{
	return (((t_entry *)node->content)->d_type);
}

void	print_entries(t_term *term)
{
	t_list	*current;

	current = term->entries;
	while (current != NULL)
	{
		ft_printf("\e[1;3%dm%s\e[m\t", get_dtype(current), get_dname(current));
		current = current->next;
	}
	ft_printf("\n");
}
