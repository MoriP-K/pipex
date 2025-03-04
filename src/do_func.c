/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_func.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:10:11 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/03/04 23:30:40 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	do_execve(t_cmd *cmd, t_proc *proc, t_fd *fd)
{
	execve(cmd->path, cmd->arr, cmd->envp);
	all_free(cmd, fd, proc);
	throw_error("execve", cmd, fd, NONE);
}

void	fail_to_fork(t_cmd *cmd, t_fd *fd, t_proc *proc)
{
	all_free(cmd, fd, proc);
	throw_error("fork", NULL, NULL, NONE);
}

void	multiple_pipe(t_cmd *cmd, t_fd *fd, t_proc *proc, int index)
{
	if (dup2(fd->pipe[index - 1][0], STDIN_FILENO) == -1)
	{
		all_free(cmd, fd, proc);
		throw_error("dup2a", cmd, fd, NONE);
	}
	if (dup2(fd->pipe[index][1], STDOUT_FILENO) == -1)
	{
		all_free(cmd, fd, proc);
		throw_error("dup2b", cmd, fd, NONE);
	}
	close(fd->pipe[index - 1][0]);
	close(fd->pipe[index][1]);
}

void	do_pipex(t_cmd *cmd, t_fd *fd, t_proc *proc)
{
	int	i;

	i = 0;
	while (i < cmd->count)
	{
		// if (i != cmd->count - 1)
		// {
			if (pipe(fd->pipe[i]) != 0)
			{
				all_free(cmd, fd, proc);
				throw_error("pipe", NULL, NULL, NONE);
			}
		// }
		proc->id[i] = fork();
		if (proc->id[i] == -1)
			fail_to_fork(cmd, fd, proc);
		else if (proc->id[i] == 0)
		{
			if (i == 0)
			{
				printf("indexR: %d\n", i);
				read_from_infile(cmd, fd, proc, i);
			}
			else if (i == cmd->count - 1)
			{
				printf("indexP: %d\n", i);
				write_to_outfile(cmd, fd, proc, i);
			}
			else
			{
				printf("indexW: %d\n", i);
				multiple_pipe(cmd, fd, proc, i);
			}
			find_cmd(cmd, fd, proc);
			close_fds(cmd, fd, NONE);
			close(fd->pipe[i][0]);
			close(fd->pipe[i][1]);
			do_execve(cmd, proc, fd);
		}
		else
		{
			close_parent_fd(i, fd, cmd->count);
			cmd->idx++;
		}
		i++;
	}
}
