/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoami <tkoami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 18:08:08 by tkoami            #+#    #+#             */
/*   Updated: 2020/12/18 09:52:22 by tkoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup(const char *s1)
{
	char		*res;
	size_t		len;
	size_t		i;

	if (s1 == NULL)
	{
		len = 0;
		if (!(res = (char*)malloc(sizeof(char) * (len + 1))))
			return (NULL);
		*res = '\0';
		return (res);
	}
	len = ft_strlen(s1);
	if (!(res = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		res[i] = s1[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	char	*str;
	char	to_find;
	size_t	i;

	if (s == NULL)
		return (NULL);
	str = (char*)s;
	to_find = (char)c;
	i = 0;
	while (str[i] != to_find)
	{
		if (str[i] == '\0')
			return (NULL);
		i++;
	}
	return (&str[i]);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*res;
	size_t	len;
	int		i;

	if (s1 == NULL)
		s1 = ft_strdup(NULL);
	if (s2 == NULL)
		s2 = ft_strdup(NULL);
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	if (!(res = (char *)malloc(len)))
		return (NULL);
	i = 0;
	while (*s1 != '\0')
		res[i++] = *s1++;
	while (*s2 != '\0')
		res[i++] = *s2++;
	res[i] = '\0';
	safe_free(s1);
	safe_free(s2);
	return (res);
}
