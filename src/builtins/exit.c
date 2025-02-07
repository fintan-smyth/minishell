/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myiu <myiu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:24:45 by myiu              #+#    #+#             */
/*   Updated: 2025/02/07 15:51:08 by myiu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../parsing/parsing.h"

void	exit_shell(t_prog *term)
{
	traverse_ptree(term->ptree, PST_ORD, free_ptree_node, NULL);
	cleanup(term);
	exit(EXIT_SUCCESS);
}