/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:44:29 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 18:59:06 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	throw_error(char *str, t_cmd *cmd, t_fd *fd, int timing)
{
	if (str)
	{
		write(STDERR_FILENO, "bash: ", 6);
		perror(str);
		// write(STDERR_FILENO, str, ft_strlen(str));
		// write(STDERR_FILENO, ": ", 2);
		// write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
		// write(STDERR_FILENO, "\n", 1);
	}
	free_cmd(cmd);
	close_fds(fd, timing);
	exit(EXIT_FAILURE);
}

void	validate_arg(int ac)
{
	if (ac != 5)
	{
		ft_putstr_fd("Error: Invalid number of arguments\n", STDERR_FILENO);
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

void	*free_array_and_added(char **arr, char **added)
{
	free_array(added);
	free_array(arr);
	return (NULL);
}

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

char	*find_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
			return (envp[i]);
		i++;
	}
	return (NULL);
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

int	is_executable_file(char *cmd)
{
	if (!cmd)
		return (0);
	if (access(cmd, X_OK) == 0)
		return (1);
	return (0);
}

void	init_cmd(int ac, char **av, char **envp, t_cmd *cmd)
{
	cmd->count = ac - 3; // ac - 3
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

void	command_not_found(t_cmd *cmd, t_fd *fd, int timing)
{
	write(STDERR_FILENO, cmd->arr[0], ft_strlen(cmd->arr[0]));
	write(STDERR_FILENO, ": command not found\n", 20);
	free_cmd(cmd);
	close_fds(fd, timing);
	exit(127);
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

void	read_from_infile(t_cmd *cmd, t_fd *fd, t_proc *proc)
{
	fd->infile = open(cmd->infile, O_RDONLY);
	if (fd->infile == -1)
	{
		free_proc(proc);
		throw_error(cmd->infile, cmd, fd, NONE);
	}
	if (dup2(fd->infile, STDIN_FILENO) == -1)
	{
		free_proc(proc);
		throw_error("dup2(file1)", cmd, fd, INFILE);
	}
	if (dup2(fd->pipe[1], STDOUT_FILENO) == -1)
	{
		free_proc(proc);
		throw_error("dup2(pipe_w)", cmd, fd, INFILE);
	}
	close_fds(fd, INFILE);
}

void	write_to_outfile(t_cmd *cmd, t_fd *fd, t_proc *proc)
{
	fd->outfile = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd->outfile == -1)
	{
		free_proc(proc);
		throw_error(cmd->outfile, cmd, fd, NONE);
	}
	if (dup2(fd->pipe[0], STDIN_FILENO) == -1)
	{
		free_proc(proc);
		throw_error("dup2(pipe_r)", cmd, fd, OUTFILE);
	}
	if (dup2(fd->outfile, STDOUT_FILENO) == -1)
	{
		free_proc(proc);
		throw_error("dup2(file2)", cmd, fd, OUTFILE);
	}
	close_fds(fd, OUTFILE);
}

void	do_execve(t_cmd *cmd, t_proc *proc, t_fd *fd)
{
	execve(cmd->path, cmd->arr, cmd->envp);
	free_proc(proc);
	throw_error("execve", cmd, fd, NONE);
}

void	close_parent_fd(int cmd_index, t_fd *fd, int cmd_count)
{
	if (cmd_index == 0)
		close(fd->pipe[1]);
	if (cmd_index == cmd_count - 1)
		close(fd->pipe[0]);
}

void	do_pipex(t_cmd *cmd, t_fd *fd, t_proc *proc)
{
	int	i;

	i = 0;
	while (i < cmd->count)
	{
		proc->id[i] = fork();
		if (proc->id[i] == -1)
		{
			all_free(cmd, proc);
			throw_error("fork", NULL, NULL, NONE);
		}
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
