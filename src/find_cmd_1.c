/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_cmd_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:17:20 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 19:18:35 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**add_slash(char **arr)
{
	int		i;
	char	**added;
	int		arr_len;

	if (!arr)
		return (NULL);
	arr_len = ft_arrlen(arr);
	added = (char **)malloc(sizeof(char *) * (arr_len + 1));
	if (!added)
		return (NULL);
	i = 0;
	while (i < arr_len)
	{
		added[i] = ft_strjoin(arr[i], "/");
		if (!added[i])
			return (free_array_and_added(arr, added));
		free(arr[i]);
		i++;
	}
	added[i] = NULL;
	free(arr);
	return (added);
}

char	*join_cmd_and_path(char *cmd, char **split_arr)
{
	int		i;
	char	*path_cmd;

	i = 0;
	while (split_arr[i])
	{
		path_cmd = ft_strjoin(split_arr[i], cmd);
		if (!path_cmd)
		{
			free_array(split_arr);
			return (NULL);
		}
		if (access(path_cmd, X_OK) == 0)
		{
			free_array(split_arr);
			return (path_cmd);
		}
		free(path_cmd);
		i++;
	}
	free_array(split_arr);
	return (NULL);
}

char	*find_cmd_path(char *cmd, char **envp)
{
	char	*path;
	char	**split_arr;

	path = find_path(envp);
	if (!path)
		return (NULL);
	split_arr = ft_split(path + 5, ':');
	split_arr = add_slash(split_arr);
	if (!split_arr)
		return (NULL);
	return (join_cmd_and_path(cmd, split_arr));
}

void	find_cmd(t_cmd *cmd, t_fd *fd, t_proc *proc)
{
	cmd->arr = ft_split(cmd->argv[cmd->idx], ' ');
	if (!cmd->arr)
		throw_error(cmd->argv[cmd->idx], NULL, fd, NONE);
	if (ft_strchr(cmd->arr[0], '/'))
	{
		if (is_executable_file(cmd->arr[0]))
			cmd->path = cmd->arr[0];
		else
		{
			free_proc(proc);
			throw_error(cmd->arr[0], cmd, fd, NONE);
		}
	}
	else
	{
		cmd->path = find_cmd_path(cmd->arr[0], cmd->envp);
		if (!cmd->path)
		{
			free_proc(proc);
			command_not_found(cmd, fd, NONE);
		}
	}
}
