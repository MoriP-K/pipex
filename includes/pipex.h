/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 23:43:52 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/02/23 15:31:02 by kmoriyam         ###   ########.fr       */
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

#endif