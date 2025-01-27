/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 17:42:44 by fsmyth            #+#    #+#             */
/*   Updated: 2024/12/02 12:10:04 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*prev;

	if (lst == NULL)
		return ;
	current = *lst;
	if (current == NULL)
		return ;
	while (current->next != NULL)
	{
		prev = current;
		current = current->next;
		del(prev->content);
		free(prev);
	}
	del(current->content);
	free(current);
	*lst = NULL;
}
