/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:00:51 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 19:28:18 by kmoriyam         ###   ########.fr       */
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

void	init_fd(t_fd *fd, t_cmd cmd)
{
	int	i;

	fd->infile = 0;
	fd->outfile = 0;
	i = 0;
	while (i < cmd.count)
		fd->pipe[i++] = 0;
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
