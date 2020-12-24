/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoami <tkoami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 17:25:31 by tkoami            #+#    #+#             */
/*   Updated: 2020/12/24 14:45:29 by tkoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		get_next_line(int fd, char **line)
{
	char			*line_feed;
	char			*buf;
	t_list			*current_lst;
	static t_list	*lst;

	if (BUFFER_SIZE <= 0 || fd <= 0)
		return (D_ERROR);
	if (!(current_lst = get_list(fd, &lst)))
		return (D_ERROR);
	buf = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!(line_feed = ft_strchr(current_lst->exstr, '\n')))
	{
		*line = ft_strdup(current_lst->exstr);
		safe_free(&(current_lst->exstr));
		if (!*line || !buf)
			return (error_processor(line, buf, &current_lst));
		//if (current_lst->eof_flag)
			//return (D_EOF);
		return (my_read(fd, buf, current_lst, line));
	}
	*line_feed = '\0';
	*line = ft_strdup(current_lst->exstr);
	safe_free(&(current_lst->exstr));
	current_lst->exstr = ft_strdup(line_feed + 1);
	free(buf);
	return (D_SUCCESS);
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
	new->eof_flag = 0;
	new->prev = NULL;
	new->next = NULL;
	new->exstr = NULL;
	return (new);
}

int		my_read(int fd, char *buf, t_list *lst, char **line)
{
	char		*line_feed;
	ssize_t		read_size;

	if (!buf)
		buf = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	read_size = read(fd, buf, BUFFER_SIZE);
	if (!buf || read_size == -1)
		return (error_processor(line, buf, &lst));
	lst->eof_flag = (read_size < BUFFER_SIZE ? 1 : 0);
	buf[read_size] = '\0';
	if ((line_feed = ft_strchr(buf, '\n')))
	{
		*line_feed = '\0';
		safe_free(&(lst->exstr));
		if (!(lst->exstr = ft_strdup(line_feed + 1)))
			return (error_processor(line, buf, &lst));
	}
	if (!(*line = ft_strjoin(line, &buf)))
		return (error_processor(line, buf, &lst));
	if (lst->eof_flag && !(line_feed))
		return (D_EOF);
	return (line_feed ? D_SUCCESS : my_read(fd, buf, lst, line));
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
