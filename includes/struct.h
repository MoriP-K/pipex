/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:19:56 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/17 09:53:28 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H
# include <sys/types.h>

typedef struct s_cmd
{
	int		count;
	char	**argv;
	char	**envp;
	char	*infile;
	char	*outfile;
	int		idx;
	char	*path;
	char	**arr;
}				t_cmd;

typedef struct s_fd
{
	int	infile;
	int	outfile;
	int pipe[2];
}				t_fd;

typedef struct s_proc
{
	pid_t	*id;
	int		status;
}			t_proc;

#endif
