/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 16:10:16 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/04 18:38:19 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_ptree	*ptree_new(t_list *pipeline, int op)
{
	t_ptree	*new;

	new = ft_calloc(1, sizeof(*new));
	new->pipeline = pipeline;
	new->op = op;
	new->left = NULL;
	new->right = NULL;
	return (new);
}

void	push_ptree_stack(t_list	**stack, t_ptree *node)
{
	if (node->op != 0)
		ft_printf("\e[32mPushing\e[m %p: %d\n", stack, node->op);
	else
		ft_printf("\e[32mPushing\e[m %p: %s\n", stack, (char *)((t_cmd *)node->pipeline->content)->tokens->content);
	ft_lstadd_back(stack, ft_lstnew(node));
}

t_ptree	*pop_ptree_stack(t_list	**stack)
{
	t_list	*current;
	t_ptree	*out;

	current = *stack;
	if (current->next == NULL)
	{
		out = (t_ptree *)current->content;
		if (out->op != 0)
			ft_printf("\e[31mPopping\e[m %p: %d\n", stack, out->op);
		else
			ft_printf("\e[31mPopping\e[m %p: %s\n", stack, (char *)((t_cmd *)out->pipeline->content)->tokens->content);
		ft_lstclear(stack, NULL);
		return (out);
	}
	while (current->next->next != NULL)
		current = current->next;
	out = (t_ptree *)current->next->content;
	if (out->op != 0)
		ft_printf("\e[31mPopping\e[m %p: %d\n", stack, out->op);
	else
		ft_printf("\e[31mPopping\e[m %p: %s\n", stack, (char *)((t_cmd *)out->pipeline->content)->tokens->content);
	ft_lstdelone(current->next, NULL);
	current->next = NULL;
	return (out);
}
