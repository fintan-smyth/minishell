/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 16:10:16 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/06 16:31:59 by myiu             ###   ########.fr       */
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
		ft_lstclear(stack, NULL);
		return (out);
	}
	while (current->next->next != NULL)
		current = current->next;
	out = (t_ptree *)current->next->content;
	ft_lstdelone(current->next, NULL);
	current->next = NULL;
	return (out);
}

void	free_ptree_node(t_ptree *ptree, void *null)
{
	(void)null;
	if (ptree->op == 0)
		free_pipeline(ptree->pipeline);
	ft_lstclear(&ptree->pids, free);
	free(ptree);
}

void	traverse_ptree(t_ptree *ptree, int order, void (*f)(t_ptree *, void *),
	void *data)
{
	if (ptree == NULL)
		return ;
	if (order == PRE_ORD)
		f(ptree, data);
	traverse_ptree(ptree->left, order, f, data);
	if (order == IN_ORD)
		f(ptree, data);
	traverse_ptree(ptree->right, order, f, data);
	if (order == PST_ORD)
		f(ptree, data);
}

void	print_ptree_node(t_ptree *ptree, void *null)
{
	(void)null;
	if (ptree->op > 0)
	{
		if (ptree->op == OP_AND)
			ft_printf("\e[1;32m&&\e[m ");
		else if (ptree->op == OP_OR)
			ft_printf("\e[1;31m||\e[m ");
		else if (ptree->op == OP_OPNPRN)
			ft_printf("\e[1;33m(e[m ");
		else if (ptree->op == OP_CLSPRN)
			ft_printf("\e[1;33m)e[m ");
	}
	else
		ft_printf("\e[1;34m%s\e[m ",
			(char *)((t_cmd *)ptree->pipeline->content)->tokens->content);
}
