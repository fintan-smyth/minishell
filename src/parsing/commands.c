/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 15:18:36 by fsmyth            #+#    #+#             */
/*   Updated: 2025/02/05 00:32:40 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd	*construct_cmd(t_list *tokens)
// Returns a memory allocated t_cmd struct containing 'tokens'.
{
	t_cmd	*cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	cmd->tokens = tokens;
	cmd->fd_out = 1;
	(cmd->pipe)[0] = -1;
	(cmd->pipe)[1] = -1;
	cmd->sep = OP_NONE;
	return (cmd);
}

int	is_cmd_sep(t_list *token)
// Checks if a token is used to separate commands.
{
	char	*text;

	text = (char *)token->content;
	if (ft_strncmp(text, "|", 2) == 0)
		return (OP_PIPE);
	if (ft_strncmp(text, "||", 3) == 0)
		return (OP_OR);
	if (ft_strncmp(text, "&&", 3) == 0)
		return (OP_AND);
	if (ft_strncmp(text, "(", 2) == 0)
		return (OP_OPNPRN);
	if (ft_strncmp(text, ")", 2) == 0)
		return (OP_CLSPRN);
	return (0);
}

void	free_cmd(void *cmdptr)
// Frees a t_cmd struct.
{
	t_cmd	*cmd;

	cmd = (t_cmd *)cmdptr;
	ft_lstclear(&cmd->tokens, free);
	free(cmd->argv);
	if (cmd->rd_in == 1)
		close(cmd->fd_in);
	if (cmd->rd_out == 1)
		close(cmd->fd_out);
	if ((cmd->pipe)[0] > 0)
	{
		close((cmd->pipe)[1]);
		close((cmd->pipe)[0]);
	}
	free(cmdptr);
}

void	free_pipeline(void *lstptr)
{
	t_list	*pipeline;

	pipeline = (t_list *)lstptr;
	ft_lstclear(&pipeline, free_cmd);
}

// t_list	*split_commands(t_list *tokens)
// // Splits a list of tokens into a list of commands to be executed in sequence.
// // Returns the list of t_cmd structs.
// {
// 	t_list	*pipeline;
// 	t_list	*cmd_list;
// 	t_list	*current_tkn;
// 	t_cmd	*cmd;
// 	int		sep;
//
// 	cmd_list = NULL;
// 	pipeline = NULL;
// 	cmd = construct_cmd(tokens);
// 	ft_lstadd_back(&pipeline, ft_lstnew(cmd));
// 	ft_lstadd_back(&cmd_list, ft_lstnew(pipeline));
// 	current_tkn = tokens;
// 	while (current_tkn != NULL)
// 	{
// 		sep = is_cmd_sep(current_tkn);
// 		if (sep == OP_PIPE && current_tkn->next != NULL)
// 		{
// 			cmd->sep = sep;
// 			cmd = construct_cmd(current_tkn->next);
// 			ft_lstadd_back(&pipeline, ft_lstnew(cmd));
// 			current_tkn->next = NULL;
// 			current_tkn = cmd->tokens;
// 		}
// 		else if (sep > OP_PIPE && current_tkn->next != NULL)
// 		{
// 			cmd->sep = sep;
// 			cmd = construct_cmd(current_tkn->next);
// 			ft_lstadd_back(&cmd_list, ft_lstnew(ft_lstnew(cmd)));
// 			cmd->condition = sep;
// 			current_tkn->next = NULL;
// 			current_tkn = cmd->tokens;
// 			pipeline = (t_list *)ft_lstlast(cmd_list)->content;
// 		}
// 		else if (is_redirect(current_tkn))
// 			encode_redirect(current_tkn);
// 		current_tkn = current_tkn->next;
// 	}
// 	return (cmd_list);
// }

t_list	*split_commands(t_list *tokens)
// Splits a list of tokens into a list of commands to be executed in sequence.
// Returns the list of t_cmd structs.
{
	t_list	*pipeline;
	t_list	*ptree_list;
	t_list	*current_tkn;
	t_list	*temp;
	t_cmd	*cmd;
	int		sep;

	ptree_list = NULL;
	pipeline = NULL;
	current_tkn = tokens;
	temp = NULL;
	sep = is_cmd_sep(current_tkn);
	while (sep > OP_PIPE)
	{
		push_ptree_stack(&ptree_list, ptree_new(NULL, sep));
		temp = current_tkn;
		current_tkn = current_tkn->next;
		ft_lstdelone(temp, free);
		if (current_tkn == NULL)
			break ;
		sep = is_cmd_sep(current_tkn);
	}
	cmd = construct_cmd(current_tkn);
	ft_lstadd_back(&pipeline, ft_lstnew(cmd));
	push_ptree_stack(&ptree_list, ptree_new(pipeline, 0));
	while (current_tkn != NULL)
	{
		sep = is_cmd_sep(current_tkn);
		if (sep == OP_PIPE && current_tkn->next != NULL)
		{
			cmd = construct_cmd(current_tkn->next);
			ft_lstadd_back(&pipeline, ft_lstnew(cmd));
			current_tkn->next = NULL;
			current_tkn = cmd->tokens;
		}
		else if (sep > OP_PIPE)
		{
			temp->next = NULL;
			while (sep > OP_PIPE)
			{
				push_ptree_stack(&ptree_list, ptree_new(NULL, sep));
				temp = current_tkn;
				current_tkn = current_tkn->next;
				ft_lstdelone(temp, free);
				if (current_tkn == NULL)
					break ;
				sep = is_cmd_sep(current_tkn);
			}
			if (current_tkn == NULL)
				break ;
			cmd = construct_cmd(current_tkn);
			push_ptree_stack(&ptree_list, ptree_new(ft_lstnew(cmd), 0));
			pipeline = ((t_ptree *)ft_lstlast(ptree_list)->content)->pipeline;
		}
		else if (is_redirect(current_tkn))
			encode_redirect(current_tkn);
		temp = current_tkn;
		current_tkn = current_tkn->next;
	}
	return (ptree_list);
}

t_ptree	*construct_parse_tree(t_list **ptree_list)
{
	t_list	*current;
	t_list	*charstack;
	t_list	*nodestack;
	t_ptree	*tree_node;

	current = *ptree_list;
	charstack = NULL;
	nodestack = NULL;
	while (current != NULL)
	{
		if (((t_ptree *)current->content)->op == OP_OPNPRN)
			push_ptree_stack(&charstack, current->content);
		else if (((t_ptree *)current->content)->op == 0)
			push_ptree_stack(&nodestack, current->content);
		else if (((t_ptree *)current->content)->op == 2
			|| ((t_ptree *)current->content)->op == 3)
		{
			while (charstack != NULL && ((t_ptree *)ft_lstlast(charstack)->content)->op != OP_OPNPRN)
			{
				tree_node = pop_ptree_stack(&charstack);
				tree_node->right = pop_ptree_stack(&nodestack);
				tree_node->left = pop_ptree_stack(&nodestack);
				push_ptree_stack(&nodestack, tree_node);
			}
			push_ptree_stack(&charstack, current->content);
		}
		else if (((t_ptree *)current->content)->op == OP_CLSPRN)
		{
			free_ptree_node(current->content);
			while (charstack != NULL && ((t_ptree *)ft_lstlast(charstack)->content)->op != OP_OPNPRN)
			{
				tree_node = pop_ptree_stack(&charstack);
				tree_node->right = pop_ptree_stack(&nodestack);
				tree_node->left = pop_ptree_stack(&nodestack);
				push_ptree_stack(&nodestack, tree_node);
			}
			free_ptree_node(pop_ptree_stack(&charstack));
		}
		current = current->next;
	}
	while (charstack != NULL)
	{
		tree_node = pop_ptree_stack(&charstack);
		tree_node->right = pop_ptree_stack(&nodestack);
		tree_node->left = pop_ptree_stack(&nodestack);
		push_ptree_stack(&nodestack, tree_node);
	}
	tree_node = (t_ptree *)(ft_lstlast(nodestack)->content);
	ft_lstclear(ptree_list, NULL);
	ft_lstclear(&nodestack, NULL);
	return (tree_node);
}

void	print_ptree_lst(t_list *ptree_list)
{
	t_list	*current;
	t_ptree *node;

	current = ptree_list;
	while (current != NULL)
	{
		node = (t_ptree *)current->content;
		if (node->op == 0)
			ft_printf("cmd: %s\n", (char *)((t_cmd *)node->pipeline->content)->tokens->content);
		else
			ft_printf("op: %d\n", node->op);
		current = current->next;
	}
}
