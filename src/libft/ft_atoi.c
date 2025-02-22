/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:05:09 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/01/11 13:40:17 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	size_t		i;
	int			sign;
	long int	result;

	i = 0;
	while (('\t' <= nptr[i] && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	sign = 1;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	result = 0;
	while (ft_isdigit((int)nptr[i]))
	{
		if (sign == 1 && result > (LONG_MAX - (nptr[i] - '0')) / 10)
			return ((int)LONG_MAX);
		else if (sign == -1 && result > (LONG_MAX - (nptr[i] - '0')) / 10)
			return ((int)LONG_MIN);
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return ((int)(sign * result));
}
