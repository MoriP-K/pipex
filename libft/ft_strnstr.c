/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:09:50 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/01/11 13:43:58 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	lit_len;

	if (!big || !little)
		return (NULL);
	if (little[0] == '\0')
		return ((char *)big);
	i = 0;
	lit_len = ft_strlen(little);
	while ((i + lit_len <= len) && big[i])
	{
		if (little[0] == big[i])
		{
			if (ft_strncmp(&big[i], little, lit_len) == 0)
				return ((char *)&big[i]);
		}
		i++;
	}
	return (NULL);
}
