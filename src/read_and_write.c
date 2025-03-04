/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_and_write.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:15:40 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/03/04 23:23:48 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	read_from_infile(t_cmd *cmd, t_fd *fd, t_proc *proc, int index)
{
	printf("read infile\n");
	fd->infile = open(cmd->infile, O_RDONLY);
	if (fd->infile == -1)
	{
		all_free(cmd, fd, proc);
		throw_error(cmd->infile, cmd, fd, NONE);
	}
	if (dup2(fd->infile, STDIN_FILENO) == -1)
	{
		all_free(cmd, fd, proc);
		throw_error("dup2(file1)", cmd, fd, INFILE);
	}
	if (dup2(fd->pipe[index][1], STDOUT_FILENO) == -1)
	{
		all_free(cmd, fd, proc);
		throw_error("dup2(pipe_w)", cmd, fd, INFILE);
	}
	close_fds(cmd, fd, INFILE);
}

void	write_to_outfile(t_cmd *cmd, t_fd *fd, t_proc *proc, int index)
{
	printf("write outfile\n");
	fd->outfile = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd->outfile == -1)
	{
		all_free(cmd, fd, proc);
		throw_error(cmd->outfile, cmd, fd, NONE);
	}
	if (dup2(fd->pipe[index - 1][0], STDIN_FILENO) == -1)
	{
		all_free(cmd, fd, proc);
		throw_error("dup2(pipe_r)", cmd, fd, OUTFILE);
	}
	if (dup2(fd->outfile, STDOUT_FILENO) == -1)
	{
		all_free(cmd, fd, proc);
		throw_error("dup2(file2)", cmd, fd, OUTFILE);
	}
	close_fds(cmd, fd, OUTFILE);
}
