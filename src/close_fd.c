/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:11:30 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/03/04 23:36:40 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	close_fds(t_cmd *cmd, t_fd *fd, t_timing timing)
{
	// int	i;

	if (fd)
	{
		// i = 1;
		// while (i < cmd->count)
		// {
		// 	close(fd->pipe[i - 1][0]);
		// 	close(fd->pipe[i][1]);
		// 	i++;
		// }
		if (timing == INFILE)
		{
			close(fd->infile);
		}
		else if (timing == OUTFILE)
		{
			close(fd->outfile);
			// close(fd->pipe[cmd->idx][0]);
			// close(fd->pipe[cmd->idx][1]);
		}
	}
}

void	close_parent_fd(int cmd_index, t_fd *fd, int cmd_count)
{
	if (cmd_index == 0)
		close(fd->pipe[cmd_index][1]);
	else if (cmd_index == cmd_count - 1)
		close(fd->pipe[cmd_index - 1][0]);
	else
	{
		close(fd->pipe[cmd_index - 1][0]);
		close(fd->pipe[cmd_index][1]);
	}
}
