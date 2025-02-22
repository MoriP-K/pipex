/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 20:50:03 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/01/07 23:21:58 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memflow(int sign, long result, char **endptr, char *nptr)
{
	if (result > (LONG_MAX - (*nptr - '0')) / 10)
	{
		if (endptr)
			*endptr = (char *)nptr + 1;
		if (sign == -1)
			return (INT_MIN);
		return (INT_MAX);
	}
	return (0);
}

int	set_endptr(long result, char **endptr, char *nptr)
{
	*endptr = nptr;
	return (result);
}

long	ft_strtol(const char *nptr, char **endptr)
{
	size_t		i;
	int			sign;
	long		result;

	i = 0;
	while (nptr[i] == '\t' || nptr[i] == '\r' || nptr[i] == ' ')
		i++;
	sign = 1;
	if (nptr[i] == '+' || nptr[i] == '-')
		if (nptr[i++] == '-')
			sign = -sign;
	result = 0;
	while (nptr[i] && ft_isdigit((int)nptr[i]))
	{
		if (ft_memflow(sign, result, endptr, (char *)&(nptr[i])))
			return (ft_memflow(sign, result, endptr, (char *)&(nptr[i])));
		result = result * 10 + (nptr[i++] - '0');
	}
	if (endptr)
		*endptr = (char *)&nptr[i];
	return (sign * result);
}
