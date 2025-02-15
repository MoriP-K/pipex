/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:44:29 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/15 23:30:50 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#define FILE_PATH "./infile"
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif

void	print_fds(int infile_fd, int *fds)
{
	printf("infile_fd = %d\n", infile_fd);
	printf("pipe(): fds[0] = %d, fds[1] = %d, STDIN_FILENO = %d, STDOUT_FILENO = %d\n", 
		fds[0], fds[1], STDIN_FILENO, STDOUT_FILENO);
}

void	throw_error(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

// void	validate_arg(int ac, char *av[])
// {
// 	if (ac < 2 || 4 < ac)
// 		throw_error(1);
// 	if (access(av[1], R_OK) == -1)
// 	{
// 		perror("access");
// 		exit(EXIT_FAILURE);
// 	}
// }

char	**add_slash(char **arr)
{
	int	i;
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
	printf("CC %s\n", path_cmd);
	return (path_cmd);
}

int	main(int ac, char *av[], char *envp[])
{
	int			infile_fd;
	int			fds[2];
	int			status;
	pid_t		pid;
	char		*infile;
	int			cmd_count;
	char		*cmd_path;
	char		**cmd_arr;

	if (ac < 2)
		return (0);
	cmd_count = ac - 2;
	// validate_arg(ac, av);

	infile  = av[1];
	infile_fd = open(infile, O_RDONLY);
	if (infile_fd < 0)
		return (0);
	if (pipe(fds) != 0)
		throw_error("pipe");
	print_fds(infile_fd, fds);
	// cmd 1
	pid = fork();
	if (pid == -1)
		throw_error("fork");
	else if (pid == 0)
	{
		printf("child1\n");
		cmd_path = find_cmd_path(av[2], envp);
		cmd_arr = ft_split(av[2], ' ');
		close(fds[0]);
		if (dup2(infile_fd, STDIN_FILENO) == -1)
			throw_error("dup2(infile)");
		if (dup2(fds[1], STDOUT_FILENO) == -1)
			throw_error("dup2(pipe)");
		close(fds[1]);
		print_fds(infile_fd, fds);
		execve(cmd_path, cmd_arr, envp);
		throw_error("execve");
	}
	else
	{
		printf("Parent1-1\n");
		waitpid(pid, &status, 0);
		printf("Parent1-2\n");
		// if (WIFEXITED(status))
		// 	printf("exit status = %d\n", WEXITSTATUS(status));
		// exit(EXIT_SUCCESS);
	}
	
	
	// cmd 2
	pid = fork();
	if (pid == -1)
		throw_error("fork");
	else if (pid == 0)
	{
		printf("child2\n");
		if (dup2(fds[0], STDIN_FILENO) == -1)
			throw_error("dup2(infile)");
		if (dup2(STDOUT_FILENO, fds[1]) == -1)
			throw_error("dup2(pipe)");
		cmd_path = find_cmd_path(av[3], envp);
		printf("%s\n", cmd_path);
		cmd_arr = ft_split(av[3], ' ');
		execve(cmd_path, cmd_arr, envp);
		throw_error("execve");
	}
	else
	{
		printf("Parent2-1\n");
		waitpid(pid, &status, 0);
		printf("Parent2-2\n");
		printf("back to parent, cp2 = %d, finished!\n", pid);
	}
	return (0);
}