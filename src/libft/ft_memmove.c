/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:41:40 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/01/11 13:39:03 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dest_p;
	unsigned char	*src_p;

	if (!dest || !src)
		return (NULL);
	dest_p = (unsigned char *)dest;
	src_p = (unsigned char *)src;
	i = 0;
	if (dest_p < src_p)
	{
		while (i < n)
		{	
			dest_p[i] = src_p[i];
			i++;
		}
	}
	else
	{
		while (n-- > 0)
			dest_p[n] = src_p[n];
	}
	return (dest);
}

// #include <string.h>
// #include <stdio.h>
// int main()
// {
// 	// メモリが重なる場合のテスト
// 	printf("dest < src\n");
// 	char str[] = "abcdefg";
// 	printf("c :%s\n", (char *)memmove(str, str + 2, 5));
// 	char str2[] = "abcdefg";
// 	printf("my:%s\n", (char *)ft_memmove(str2, str2 + 2, 5));
// 	printf("dest > src\n");
// 	char str3[] = "abcdefg";
// 	printf("c :%s\n", (char *)memmove(str3 + 2, str3, 4));
// 	char str4[] = "abcdefg";
// 	printf("my:%s\n", (char *)ft_memmove(NULL, str4, 4));
// }