/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:44:29 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 20:12:37 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	validate_arg(int ac)
{
	if (ac != 5)
	{
		ft_putstr_fd("Error: Invalid number of arguments\n", STDERR_FILENO);
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

void	wait_child_process(t_proc proc, t_cmd cmd)
{
	int	i;

	i = 0;
	while (i < cmd.count)
	{
		waitpid(proc.id[i], &proc.status, 0);
		i++;
	}
}

int	main(int ac, char *av[], char *envp[])
{
	t_cmd	cmd;
	t_fd	fd;
	t_proc	proc;

	validate_arg(ac);
	init_cmd(ac, av, envp, &cmd);
	init_fd(&fd, cmd);
	init_proc(&proc, cmd);
	if (pipe(fd.pipe) != 0)
	{
		all_free(&cmd, &proc);
		throw_error("pipe", NULL, NULL, NONE);
	}
	do_pipex(&cmd, &fd, &proc);
	wait_child_process(proc, cmd);
	free_proc(&proc);
	close_fds(&fd, NONE);
	return (0);
}
