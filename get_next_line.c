/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoami <tkoami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 17:25:31 by tkoami            #+#    #+#             */
/*   Updated: 2020/12/09 09:37:36 by tkoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		get_next_line(int fd, char **line)
{
	char			*lf;
	t_list			*current_lst;
	static t_list	*lst;

	if (!(current_lst = get_list(fd, &lst)))
		return (-1);
	if (!(lf = ft_strchr(current_lst->exstr, '\n')))
	{
		*line = ft_strdup(current_lst->exstr);
		if (current_lst->eof_flag)
			return (0);
		return (my_read(fd, current_lst, line));
	}
	*lf = '\0';
	*line = ft_strdup(current_lst->exstr);
	current_lst->exstr = lf++;
	return (1);
}

t_list	*get_list(int fd, t_list **lst)
{
	t_list	*current_lst;

	if (!*lst)
		return(list_init(fd));
	while((*lst)->next)
	{
		if ((*lst)->fd == fd)
		{
			current_lst = *lst;
			while ((*lst)->prev)
				*lst = (*lst)->prev;
			return (current_lst);
		}
		*lst = (*lst)->next;
	}
	if (!(current_lst = list_init(fd)))
		return (NULL);
	(*lst)->next = current_lst;
	current_lst->prev = (*lst);
	return(current_lst);
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

int		my_read(int fd, t_list *lst, char **line)
{
	char	buf[BUFFER_SIZE + 1];
	char 	*lf;
	char	*tmp;
	ssize_t	read_size;

	read_size = read(fd, buf, BUFFER_SIZE);
	buf[read_size] = '\0';
	if (read_size < BUFFER_SIZE)
		lst->eof_flag = 1;
	if((lf = ft_strchr(buf, '\n')))
	{
		*lf = '\0';
		tmp = ft_strjoin(*line, buf);
		free(*line);
		line = &tmp;
		lst->exstr = lf++;
		return (1);
	}
	tmp = ft_strjoin(*line, buf);
	free(*line);
	line = &tmp;
	if (lst->eof_flag)
		return (0);
	return (my_read(fd, lst, line));
}