/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_func.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:10:11 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 19:30:32 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	do_execve(t_cmd *cmd, t_proc *proc, t_fd *fd)
{
	execve(cmd->path, cmd->arr, cmd->envp);
	free_proc(proc);
	throw_error("execve", cmd, fd, NONE);
}

void	fail_to_fork(t_cmd *cmd, t_proc *proc)
{
	all_free(cmd, proc);
	throw_error("fork", NULL, NULL, NONE);
}

void	do_pipex(t_cmd *cmd, t_fd *fd, t_proc *proc)
{
	int	i;

	i = 0;
	while (i < cmd->count)
	{
		proc->id[i] = fork();
		if (proc->id[i] == -1)
			fail_to_fork(cmd, proc);
		else if (proc->id[i] == 0)
		{
			if (i == 0)
				read_from_infile(cmd, fd, proc);
			if (i == cmd->count - 1)
				write_to_outfile(cmd, fd, proc);
			find_cmd(cmd, fd, proc);
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
