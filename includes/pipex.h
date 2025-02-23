/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:43:52 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 19:30:06 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <errno.h>
# include <sys/wait.h>
# include <string.h> 
# include "../src/libft/libft.h"
# include "struct.h"

typedef enum s_timing {
	NONE = 0,
	INFILE = 1,
	OUTFILE = 2,
}			t_timing;

void	init_cmd(int ac, char **av, char **envp, t_cmd *cmd);
void	init_fd(t_fd *fd, t_cmd cmd);
void	init_proc(t_proc *proc, t_cmd cmd);
void	free_array(char	**array);
void	free_cmd(t_cmd *cmd);
void	free_proc(t_proc *proc);
void	all_free(t_cmd *cmd, t_proc *proc);
void	*free_array_and_added(char **arr, char **added);
void	command_not_found(t_cmd *cmd, t_fd *fd, int timing);
void	throw_error(char *str, t_cmd *cmd, t_fd *fd, int timing);
void	validate_arg(int ac);
void	do_execve(t_cmd *cmd, t_proc *proc, t_fd *fd);
void	do_pipex(t_cmd *cmd, t_fd *fd, t_proc *proc);
void	fail_to_fork(t_cmd *cmd, t_proc *proc);
void	close_fds(t_fd *fd, t_timing timing);
void	close_parent_fd(int cmd_index, t_fd *fd, int cmd_count);
void	read_from_infile(t_cmd *cmd, t_fd *fd, t_proc *proc);
void	write_to_outfile(t_cmd *cmd, t_fd *fd, t_proc *proc);
void	find_cmd(t_cmd *cmd, t_fd *fd, t_proc *proc);
char	*find_cmd_path(char *cmd, char **envp);
char	*join_cmd_and_path(char *cmd, char **split_arr);
char	**add_slash(char **arr);
int		is_executable_file(char *cmd);
char	*find_path(char **envp);

#endif