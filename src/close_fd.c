/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:11:30 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 19:12:00 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	close_fds(t_fd *fd, t_timing timing)
{
	if (fd)
	{
		close(fd->pipe[0]);
		close(fd->pipe[1]);
		if (timing == INFILE)
		{
			close(fd->infile);
		}
		else if (timing == OUTFILE)
		{
			close(fd->outfile);
		}
	}
}

void	close_parent_fd(int cmd_index, t_fd *fd, int cmd_count)
{
	if (cmd_index == 0)
		close(fd->pipe[1]);
	if (cmd_index == cmd_count - 1)
		close(fd->pipe[0]);
}
