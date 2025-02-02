/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 13:48:02 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/01/11 13:41:38 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s_p;
	size_t			i;

	if (!s)
		return (NULL);
	i = 0;
	s_p = (unsigned char *)s;
	while (i < n)
	{
		if (s_p[i] == (unsigned char)c)
			return ((void *)&s_p[i]);
		i++;
	}
	return (NULL);
}
