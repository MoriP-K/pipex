/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:44:29 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/09 00:16:09 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include <stdlib.h>
#include <errno.h>
#define FILE_PATH "./infile"
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 42
#endif
int	main(int ac, char *av[])
{
	int		input_fd;
	int		pipe_fds[2];
	int		status;
	pid_t	pid;
	char	*env[] = {"PATH=/bin:/usr/bin", "HOME=/home/kmoriyam", NULL};
	char	*file_path;

	if (ac < 2)
		return (0);
	input_fd = open(av[1], O_RDONLY);
	if (input_fd < 0)
		return (0);
	file_path  = av[1];
	if (pipe(pipe_fds) != 0)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	printf("input_fd = %d\n", input_fd);
	printf("pipe(): fds[0] = %d, fds[1] = %d\n", pipe_fds[0], pipe_fds[1]);
	pid = fork();
	if (pid == -1)
	{
		perror("fork()");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		close(pipe_fds[1]);
		if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(pipe_fds[0]);
		printf("child going...\n");
		printf("child: pid = %d\n", pid);
		execve("/bin/cat", av, env);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipe_fds[0]);
		printf("parent going..., wait child(pid = %d)\n--------------------\n", pid);
		waitpid(pid, &status, 0);
		printf("\nchild(pid = %d) finished!\n", pid);
		if (WIFEXITED(status))
			printf("exit status = %d\n", WEXITSTATUS(status));
		close(pipe_fds[1]);
		exit(EXIT_SUCCESS);
	}

	return (0);
}