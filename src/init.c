/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:00:51 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/03/04 23:36:00 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	init_cmd(int ac, char **av, char **envp, t_cmd *cmd)
{
	cmd->count = ac - 3;
	cmd->argv = av;
	cmd->envp = envp;
	cmd->infile = av[1];
	cmd->outfile = av[ac - 1];
	cmd->idx = 2;
	cmd->path = NULL;
	cmd->arr = NULL;
}

void	init_fd(t_fd *fd, t_cmd *cmd)
{
	int	i;
	int	j;

	fd->infile = -1;
	fd->outfile = -1;
	fd->pipe = malloc(sizeof(int *) * cmd->count);
	if (!fd->pipe)
		throw_error("malloc", cmd, fd, NONE);
	i = 0;
	while (i < cmd->count)
	{
		fd->pipe[i] = malloc(sizeof(int) * 2);
		if (!fd->pipe[i])
		{
			free_pipe(fd->pipe, cmd->count);
			throw_error("malloc", cmd, fd, NONE);
		}
		j = 0;
		while (j < 2)
			fd->pipe[i][j++] = 0;
		i++;
	}
}

void	init_proc(t_proc *proc, t_cmd cmd)
{
	int	i;

	proc->id = malloc(sizeof(int) * cmd.count);
	if (!proc->id)
		throw_error("malloc", NULL, NULL, NONE);
	i = 0;
	while (i < cmd.count)
		proc->id[i++] = 0;
	proc->status = 0;
}
