/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmoriyam <kmoriyam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:15:08 by kmoriyam          #+#    #+#             */
/*   Updated: 2025/01/11 13:42:41 by kmoriyam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_split_array(char **array, size_t words_allocated)
{
	size_t	i;

	i = 0;
	while (i < words_allocated)
	{
		if (array[i])
			free(array[i]);
		i++;
	}
	free(array);
}

static char	*ft_strndup(const char *start, const char *end)
{
	char	*dest;
	size_t	len;
	size_t	index;

	len = end - start;
	dest = (char *)malloc(len + 1);
	if (!dest)
		return (NULL);
	index = 0;
	while (start < end)
	{
		dest[index] = *start;
		index++;
		start++;
	}
	dest[index] = '\0';
	return (dest);
}

static int	allocated_word(char **array, const char *start, const char *end,
		size_t index)
{
	array[index] = ft_strndup(start, end);
	if (!array[index])
	{
		free_split_array(array, index);
		return (0);
	}
	return (1);
}

static size_t	ft_word_count(const char *str, char delimeter)
{
	size_t	i;
	size_t	word_count;
	int		in_word;

	i = 0;
	word_count = 0;
	in_word = 0;
	while (str[i])
	{
		if (str[i] != delimeter)
		{
			if (in_word == 0)
			{
				in_word = 1;
				word_count++;
			}
		}
		else
			in_word = 0;
		i++;
	}
	return (word_count);
}

char	**ft_split(char const *s, char c)
{
	const char	*start;
	char		**array;
	size_t		word_index;

	if (!s)
		return (NULL);
	array = (char **)malloc((ft_word_count(s, c) + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	word_index = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s)
		{
			start = s;
			while (*s && *s != c)
				s++;
			if (!allocated_word(array, start, s, word_index++))
				return (NULL);
		}
	}
	array[word_index] = NULL;
	return (array);
}
