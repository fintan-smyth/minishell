/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsmyth <fsmyth@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:58:13 by fsmyth            #+#    #+#             */
/*   Updated: 2025/01/27 18:29:15 by fsmyth           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo(int argc, char **argv)
{
	int		i;
	int		nl;

	if (argc < 2)
		write(1, "\n", 1);
	else
	{
		i = 0;
		nl = 1;
		if (ft_strncmp(argv[1], "-n", 3) == 0)
		{
			nl = 0;
			i++;
		}
		while (argv[++i] != NULL)
		{
			if (i != 2 - nl)
				write(1, " ", 1);
			ft_putstr_fd(argv[i], 1);
		}
		if (nl == 1)
			write(1, "\n", 1);
		else if (i != 2)
			ft_putstr_fd("\e[7m%\e[m", 1);
	}
}
