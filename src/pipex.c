/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:44:29 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/16 21:17:35 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	print_fds(int infile_fd, int *fds)
{
	printf("infile_fd = %d\n", infile_fd);
	printf("pipe(): fds[0] = %d, fds[1] = %d, STDIN_FILENO = %d, STDOUT_FILENO = %d\n",
		fds[0], fds[1], STDIN_FILENO, STDOUT_FILENO);
}

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

char	*find_cmd_path(char *av, char *envp[])
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
		path_cmd = ft_strjoin(split_arr[i], av);
		if (access(path_cmd, X_OK) == 0)
			break ;
		i++;
	}
	return (path_cmd);
}

void	init_cmd(int ac, t_cmd *cmd)
{
	cmd->count = ac - 2;
	cmd->path = NULL;
	cmd->arr = NULL;
}

void	init_fd(t_fd *fd)
{
	fd->infile = 0;
	fd->outfile = 0;
	fd->pipe[0] = 0;
	fd->pipe[1] = 0;
}

void	init_proc(int ac, t_proc *proc)
{
	proc->id = malloc(sizeof(int) * (ac - 3));
	if (!proc->id)
		throw_error("malloc");
	proc->status = 0;
}

int	main(int ac, char *av[], char *envp[])
{
	int infile_fd;
	int outfile_fd;
	// t_cmd	cmd;
	// t_fd	fd;
	// t_proc	proc;
	int fds[2];
	int status;
	char *cmd_path;
	char **cmd_arr;

	// validate_arg(ac);
	// init_cmd(ac, &cmd);
	// init_fd(&fd);
	// init_proc(ac, &proc);
	if (pipe(fds) != 0)
		throw_error("pipe");

	// cmd 1
	int pid1 = fork();
	if (pid1 == -1)
		throw_error("fork");
	else if (pid1 == 0)
	{
		printf("child1\n");
		infile_fd = open(av[1], O_RDONLY);
		if (infile_fd == -1)
			throw_error("infile");
		cmd_arr = ft_split(av[2], ' ');
		cmd_path = find_cmd_path(cmd_arr[0], envp);
		close(fds[0]);
		if (dup2(infile_fd, STDIN_FILENO) == -1)
			throw_error("dup2(infile)");
		close(infile_fd);
		if (dup2(fds[1], STDOUT_FILENO) == -1)
			throw_error("dup2(pipe_w)");
		close(fds[1]);
		execve(cmd_path, cmd_arr, envp);
		throw_error("execve1");
	}
	else
	{
		close(fds[1]);
		printf("Parent1\n");
	}

	// cmd 2
	int pid2 = fork();
	if (pid2 == -1)
		throw_error("fork");
	else if (pid2 == 0)
	{
		printf("child2\n");
		cmd_arr = ft_split(av[3], ' ');
		cmd_path = find_cmd_path(cmd_arr[0], envp);
		close(fds[1]);
		if (dup2(fds[0], STDIN_FILENO) == -1)
			throw_error("dup2(pipe_r)");
		close(fds[0]);
		outfile_fd = open(av[ac - 1], O_WRONLY | O_CREAT, 0644);
		if (outfile_fd == -1)
			throw_error("outfile");
		if (dup2(outfile_fd, STDOUT_FILENO) == -1)
			throw_error("dup2(outfile)");
		close(outfile_fd);
		execve(cmd_path, cmd_arr, envp);
		throw_error("execve2");
	}
	else
	{
		close(fds[0]);
		printf("Parent2\n");
	}
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	printf("pid1 = %d, pid2 = %d\n", pid1, pid2);
	return (0);
}
