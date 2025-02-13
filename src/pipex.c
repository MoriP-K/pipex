/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:44:29 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/13 21:42:25 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include <stdlib.h>
#include <errno.h>
#define FILE_PATH "./infile"
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif

// void	throw_error(int	error_no)
// {
// 	if (error_no == 1)
// 		exit(EXIT_FAILURE);
// }

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

int	main(int ac, char *av[], char *envp[])
{
	int		input_fd;
	int		pipe_fds[2];
	int		status;
	pid_t	pid;
	char	*env[] = {"PATH=/bin:/usr/bin", "HOME=/home/kmoriyam", NULL};
	char	*file_path;
	ssize_t		byte_read;
	char	buf[BUFFER_SIZE];

	if (ac < 2)
		return (0);
	// validate_arg(ac, av);
	input_fd = open(av[1], O_RDONLY);
	if (input_fd < 0)
		return (0);
	file_path  = av[1];
	if (pipe(pipe_fds) != 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	// printf("input_fd = %d\n", input_fd);
	// printf("pipe(): fds[0] = %d, fds[1] = %d\n", pipe_fds[0], pipe_fds[1]);
	pid = fork();
	if (pid == -1)
	{
		perror("fork()");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(pipe_fds[1]);
		// printf("child going..., pid = %d\n", pid);
		// while (1)
		// {
		// 	byte_read = read(pipe_fds[0], buf, BUFFER_SIZE);
		// 	if (byte_read == -1)
		// 	{
		// 		perror("read");
		// 		exit(EXIT_FAILURE);
		// 	}
		// 	if (byte_read == 0)
		// 		break ;
		// 	if (write(STDOUT_FILENO, buf, byte_read) != byte_read)
		// 	{
		// 		perror("write");
		// 		exit(EXIT_FAILURE);
		// 	}
		// }
		// write(STDOUT_FILENO, "\n", 1);
		// close(STDOUT_FILENO);
		// exit(EXIT_SUCCESS);
		if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		printf("pfd = %d\n", pipe_fds[0]);
		close(pipe_fds[0]);
		// printf("child going...\n");
		// printf("child: pid = %d\n", pid);
		execve("/bin/cat", av, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipe_fds[0]);
		// if (dup2(STDOUT_FILENO, pipe_fds[1]) == -1)
		// {
		// 	perror("dup2");
		// 	exit(EXIT_FAILURE);
		// }
		// close(STDOUT_FILENO);
		while (1)
		{
			byte_read = read(input_fd, buf, BUFFER_SIZE);
			if (byte_read == -1)
			{
				perror("read");
				exit(EXIT_FAILURE);
			}
			if (byte_read == 0)
				break ;
			if (write(pipe_fds[1], buf, byte_read) != byte_read)
			{
				perror("write");
				break ;
			}
		}
		// printf("parent going..., wait child(pid = %d)\n\n", pid);
		// if (write(pipe_fds[1], av[1], ft_strlen(av[1])) != ft_strlen(av[1]))
		// {
		// 	perror("write");
		// 	exit(EXIT_FAILURE);
		// }
		close(input_fd);
		close(pipe_fds[1]);
		waitpid(pid, &status, 0);
		printf("\nback to parent, child process finished!\n", pid);
		// if (WIFEXITED(status))
		// 	printf("exit status = %d\n", WEXITSTATUS(status));
		exit(EXIT_SUCCESS);
	}
	return (0);
}