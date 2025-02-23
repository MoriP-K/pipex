/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_any.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:04:14 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 19:05:15 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_array(char	**array)
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

void	all_free(t_cmd *cmd, t_proc *proc)
{
	free_proc(proc);
	free_cmd(cmd);
}

void	*free_array_and_added(char **arr, char **added)
{
	free_array(added);
	free_array(arr);
	return (NULL);
}
