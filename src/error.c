/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:05:45 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 23:22:15 by kmoriyam         ###   ########.fr       */
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
	char	*error_msg;
	char	*system_error;
	int		error_len;

	if (str)
	{
		system_error = strerror(errno);
		error_len = ft_strlen("bash: ") + ft_strlen(str) + ft_strlen(": ") \
			+ ft_strlen(system_error) + ft_strlen("\n") + 1;
		error_msg = malloc(sizeof(char) * error_len);
		if (!error_msg)
			throw_error("malloc", cmd, fd, NONE);
		error_msg[0] = '\0';
		ft_strlcat(error_msg, "bash: ", error_len);
		ft_strlcat(error_msg, str, error_len);
		ft_strlcat(error_msg, ": ", error_len);
		ft_strlcat(error_msg, system_error, error_len);
		ft_strlcat(error_msg, "\n", error_len);
		write(STDERR_FILENO, error_msg, ft_strlen(error_msg));
		free(error_msg);
	}
	free_cmd(cmd);
	close_fds(fd, timing);
	exit(EXIT_FAILURE);
}
