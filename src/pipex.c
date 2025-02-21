/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:44:29 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/21 23:44:46 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	throw_error(char *str)
{
	if (str)
		perror(str);
	exit(EXIT_FAILURE);
}

void	validate_arg(int ac)
{
	if (ac < 2 || 5 < ac)
		throw_error(NULL);
}

char	**add_slash(char **arr)
{
	int		i;
	char	**added;
	int		arr_len;

	arr_len = ft_arrlen(arr);
	added = (char **)malloc(sizeof(char *) * (arr_len + 1));
	if (!added)
		return (NULL);
	i = 0;
	while (i < arr_len)
	{
		added[i] = ft_strjoin(arr[i], "/");
		i++;
	}
	return (added);
}

char	*find_cmd_path(char *cmd, char *envp[])
{
	int		i;
	char	*path;
	char	**split_arr;
	char	*path_cmd;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 4) == 0)
			path = envp[i];
		i++;
	}
	split_arr = ft_split(path + 5, ':');
	split_arr = add_slash(split_arr);
	i = 0;
	while (split_arr[i])
	{
		path_cmd = ft_strjoin(split_arr[i], cmd);
		if (access(path_cmd, X_OK) == 0)
			return (path_cmd);
		i++;
	}
	return (NULL);
}

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

void	init_proc(int ac, t_proc *proc, t_cmd cmd)
{
	int	i;
	proc->id = malloc(sizeof(int) * (ac - 3));
	if (!proc->id)
		throw_error("malloc");
	i = 0;
	while (i < cmd.count)
		proc->id[i++] = 0;
	proc->status = 0;
}

void	free_proc(t_proc *proc)
{
	if (proc->id)
		free(proc->id);
}

void	wait_child_process(t_proc proc, t_cmd cmd)
{
	int	i;

	i = 0;
	while (i < cmd.count)
	{
		waitpid(proc.id[i], &proc.status, 0);
		// printf("\npid%d = %d\n", i, proc.id[i]);
		i++;
	}
}

void	command_not_found(char *cmd)
{
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(127);
}

int	main(int ac, char *av[], char *envp[])
{
	t_cmd	cmd;
	t_fd	fd;
	t_proc	proc;
	int		i;

	// validate_arg(ac);
	init_cmd(ac, av, envp, &cmd);
	init_fd(&fd, cmd);
	init_proc(ac, &proc, cmd);
	if (pipe(fd.pipe) != 0)
		throw_error("pipe");

	// cmd 1
	i = 0;
	while (i < cmd.count)
	{
		proc.id[i] = fork();
		if (proc.id[i] == -1)
			throw_error("fork");
		else if (proc.id[i] == 0)
		{
			// printf("\nchild%d, pid = %d\n", i, proc.id[i]);
			cmd.arr = ft_split(cmd.argv[cmd.idx], ' ');
			// printf("cmd.index = %d\n", cmd.idx);
			cmd.path = find_cmd_path(cmd.arr[0], cmd.envp);
			if (!cmd.path)
				command_not_found(cmd.arr[0]);
			// printf("path = %s\n", cmd.path);
			// for (int j = 0; cmd.arr[j]; j++)
			// 	printf("cmd.arr%d = %s\n", i, cmd.arr[j]);
			if (i == 0)
			{
				fd.infile = open(cmd.infile, O_RDONLY);
				if (fd.infile == -1)
					throw_error("infile");
				// printf("pfd[0] = %d, pfd[1] = %d, infile fd = %d\n", fd.pipe[0], fd.pipe[1], fd.infile);
				close(fd.pipe[0]);
				if (dup2(fd.infile, STDIN_FILENO) == -1)
					throw_error("dup2(infile)");
				close(fd.infile);
				if (dup2(fd.pipe[1], STDOUT_FILENO) == -1)
					throw_error("dup2(pipe_w)");
				close(fd.pipe[1]);
			}
			if (i == cmd.count - 1)
			{
				close(fd.pipe[1]);
				if (dup2(fd.pipe[0], STDIN_FILENO) == -1)
					throw_error("dup2(pipe_r)");
				close(fd.pipe[0]);
				fd.outfile = open(cmd.outfile, O_WRONLY);
				if (fd.outfile == -1)
					throw_error("outfile");
				if (dup2(fd.outfile, STDOUT_FILENO) == -1)
					throw_error("dup2(outfile)");
				close(fd.outfile);
			}
			execve(cmd.path, cmd.arr, cmd.envp);
			throw_error("execve");
		}
		else
		{
			if (i == 0)
				close(fd.pipe[1]);
			if (i == cmd.count - 1)
				close(fd.pipe[0]);
			cmd.idx++;
			// printf("Parent%d, pid = %d\n", i, proc.id[i]);
		}
		i++;
	}

	wait_child_process(proc, cmd);
	free_proc(&proc);
	return (0);
}
