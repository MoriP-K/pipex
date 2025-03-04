/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_any.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:04:14 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/03/04 23:19:29 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->path)
			free(cmd->path);
		if (cmd->arr)
			free_array(cmd->arr);
	}
}

void	free_proc(t_proc *proc)
{
	if (proc && proc->id)
		free(proc->id);
}

void	free_pipe(int **pipe, int count)
{
	int	i;

	if (pipe)
	{
		i = 0;
		while (i < count)
		{
			free(pipe[i]);
			i++;
		}
		free(pipe);
	}
}

void	free_fd(t_fd *fd, t_cmd *cmd)
{
	if (fd->pipe)
		free_pipe(fd->pipe, cmd->count);
}

void	all_free(t_cmd *cmd, t_fd *fd, t_proc *proc)
{
	free_cmd(cmd);
	free_fd(fd, cmd);
	free_proc(proc);
}

void	*free_array_and_added(char **arr, char **added)
{
	free_array(added);
	free_array(arr);
	return (NULL);
}
