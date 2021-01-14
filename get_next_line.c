/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoami <tkoami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 17:25:31 by tkoami            #+#    #+#             */
/*   Updated: 2021/01/14 13:33:24 by tkoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		get_next_line(int fd, char **line)
{
	char			*line_feed;
	char			*buf;
	t_list			*current_lst;
	static t_list	*lst;
	ssize_t			rc;

	if (BUFFER_SIZE <= 0 || fd <= 0)
		return (D_ERROR);
	if (!(current_lst = get_list(fd, &lst)))
		return (D_ERROR);
	if (!(buf = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (D_ERROR);
	if ((line_feed = ft_strchr(current_lst->exstr, '\n')))
		*line_feed = '\0';
	if (!(*line = ft_strdup(current_lst->exstr)))
		error_processor(line, buf, &current_lst);
	if (line_feed)
	{
		line_feed = ft_strdup(line_feed + 1);
		safe_free(&(current_lst->exstr));
		current_lst->exstr = line_feed;
		safe_free(&buf);
		return (D_SUCCESS);
	}
	if ((rc = my_read(fd, &buf, current_lst, line)) < 0)
		return (error_processor(line, buf, &(current_lst)));
	safe_free(&buf);
	return (rc ? D_SUCCESS : D_EOF);
}

t_list	*get_list(int fd, t_list **lst)
{
	t_list	*current_lst;

	while (*lst)
	{
		if ((*lst)->fd == fd)
		{
			current_lst = *lst;
			while ((*lst)->prev)
				*lst = (*lst)->prev;
			return (current_lst);
		}
		if ((*lst)->next == NULL)
		{
			if (!(current_lst = list_init(fd)))
				return (NULL);
			(*lst)->next = current_lst;
			current_lst->prev = (*lst);
			while ((*lst)->prev)
				*lst = (*lst)->prev;
			return (current_lst);
		}
		*lst = (*lst)->next;
	}
	return ((*lst) = list_init(fd));
}

t_list	*list_init(int fd)
{
	t_list	*new;

	if (!(new = (t_list*)malloc(sizeof(t_list))))
		return (NULL);
	new->fd = fd;
	new->prev = NULL;
	new->next = NULL;
	new->exstr = NULL;
	return (new);
}

int		my_read(int fd, char **buf, t_list *lst, char **line)
{
	char		*line_feed;
	char		*tmp;
	ssize_t		rc;

	while ((rc = read(fd, *buf, BUFFER_SIZE)) > 0)
	{
		(*buf)[rc] = '\0';
		if ((line_feed = ft_strchr(*buf, '\n')))
		{
			*line_feed = '\0';
			if (!(lst->exstr = ft_strdup(line_feed + 1)))
				return (error_processor(line, *buf, &lst));
			tmp = *line;
			if (!(*line = ft_strjoin(tmp, *buf)))
				return (error_processor(line, *buf, &lst));
			safe_free(&tmp);
			return (rc);
		}
		tmp = *line;
		if (!(*line = ft_strjoin(tmp, *buf)))
			return (error_processor(line, *buf, &lst));
		safe_free(&tmp);
	}
	return (rc);
}

int		error_processor(char **line, char *buf, t_list **lst)
{
	free(*line);
	free(buf);
	free((*lst)->exstr);
	if ((*lst)->prev)
		(*lst)->prev->next = (*lst)->next;
	if ((*lst)->next)
		(*lst)->next->prev = (*lst)->prev;
	free(*lst);
	return (D_ERROR);
}
