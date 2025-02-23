/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:05:45 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 19:14:44 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	command_not_found(t_cmd *cmd, t_fd *fd, int timing)
{
	write(STDERR_FILENO, cmd->arr[0], ft_strlen(cmd->arr[0]));
	write(STDERR_FILENO, ": command not found\n", 20);
	free_cmd(cmd);
	close_fds(fd, timing);
	exit(127);
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
