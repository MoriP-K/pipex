/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 18:00:40 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/01/02 18:07:55 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_arrlen(char **array)
{
	size_t	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}
