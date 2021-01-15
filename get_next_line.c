/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoami <tkoami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 17:25:31 by tkoami            #+#    #+#             */
/*   Updated: 2021/01/15 09:39:24 by tkoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		get_next_line(int fd, char **line)
{
	char			*line_feed;
	char			*buf;
	static t_list	*lst;
	ssize_t			rc;

	if (BUFFER_SIZE <= 0 || fd <= 0)
		return (D_ERROR);
	if (!(lst = get_list(fd, lst)))
		return (D_ERROR);
	if (!(buf = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (D_ERROR);
	if ((line_feed = ft_strchr(lst->exstr, '\n')))
		*line_feed = '\0';
	if (!(*line = ft_strdup(lst->exstr)))
		error_processor(line, buf, lst, D_ERROR);
	if (line_feed)
	{
		line_feed = ft_strdup(line_feed + 1);
		safe_free(&(lst->exstr));
		lst->exstr = line_feed;
		rc = 1;
	}
	else if ((rc = my_read(fd, &buf, lst, line)) < 0)
		return (error_processor(line, buf, lst, D_ERROR));
	safe_free(&buf);
	return (rc ? D_SUCCESS : error_processor(line, buf, lst, D_EOF));
}

t_list	*get_list(int fd, t_list *lst)
{
	t_list *new;
	
	while (lst && lst->next)
	{
		if (lst->fd == fd)
			return lst;
		lst = lst->next;
	}
	if (!(new = (t_list*)malloc(sizeof(t_list))))
			return (NULL);
	new->fd = fd;
	new->prev = NULL;
	new->next = NULL;
	new->exstr = NULL;
	if (lst)
	{
		new->prev = lst;
		lst = new;
		return lst;
	}
	lst = new;
	return (lst);
}

int		my_read(int fd, char **buf, t_list *lst, char **line)
{
	char		*line_feed;
	char		*tmp;
	ssize_t		rc;

	while ((rc = read(fd, *buf, BUFFER_SIZE)) > 0)
	{
		(*buf)[rc] = '\0';
		safe_free(&(lst->exstr));
		if ((line_feed = ft_strchr(*buf, '\n')))
		{
			*line_feed = '\0';
			if (!(lst->exstr = ft_strdup(line_feed + 1)))
				return (error_processor(line, *buf, lst, D_ERROR));
			tmp = *line;
			if (!(*line = ft_strjoin(tmp, *buf)))
				return (error_processor(line, *buf, lst, D_ERROR));
			safe_free(&tmp);
			return (rc);
		}
		tmp = *line;
		if (!(*line = ft_strjoin(tmp, *buf)))
			return (error_processor(line, *buf, lst, D_ERROR));
		safe_free(&tmp);
	}
	return (rc);
}

int		error_processor(char **line, char *buf, t_list *lst, int res)
{
	t_list *tmp;

	tmp = lst;
	if (lst->prev)
		while (lst->prev)
			lst = lst->prev;
	else if (lst->next)
		lst = lst->next;
	if (res == -1)
		free(*line);
	free(buf);
	free(tmp->exstr);
	if (tmp->prev)
		tmp->prev->next = tmp->next;
	if (tmp->next)
		tmp->next->prev = tmp->prev;
	free(tmp);
	tmp = NULL;
	return (res);
}
