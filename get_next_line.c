/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoami <tkoami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 17:25:31 by tkoami            #+#    #+#             */
/*   Updated: 2020/12/16 23:28:11 by tkoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		get_next_line(int fd, char **line)
{
	char			*lf;
	char			*tmp;
	t_list			*current_lst;
	static t_list	*lst;

	if (!(current_lst = get_list(fd, &lst)))
		return (-1);
	if (!(lf = ft_strchr(current_lst->exstr, '\n')))
	{
		if (!(*line = ft_strdup(current_lst->exstr)))
			return (-1);
		if (current_lst->eof_flag)
			return (0);
		return (my_read(fd, current_lst, line));
	}
	*lf++ = '\0';
	*line = ft_strdup(current_lst->exstr);
	tmp = ft_strdup(lf);
	free(current_lst->exstr);
	current_lst->exstr = tmp;
	return (1);
}

t_list	*get_list(int fd, t_list **lst)
{
	t_list	*current_lst;

	if (!*lst)
	{
		if (!(*lst = list_init(fd)))
			return (NULL);
		return(*lst);
	}
	while (1)
	{
		if ((*lst)->fd == fd)
		{
			current_lst = *lst;
			while ((*lst)->prev)
				*lst = (*lst)->prev;
			return (current_lst);
		}
		if ((*lst)->next == NULL)
			break;
		*lst = (*lst)->next;
	}
	if (!(current_lst = list_init(fd)))
		return (NULL);
	(*lst)->next = current_lst;
	current_lst->prev = (*lst);
	while ((*lst)->prev)
		*lst = (*lst)->prev;
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
	if (!(new->exstr = (char*)malloc(sizeof(char))))
		return (NULL);
	new->exstr = "";
	return (new);
}

int		my_read(int fd, t_list *lst, char **line)
{
	char	*buf;
	char 	*lf;
	char	*tmp;
	int		eol_flag;
	ssize_t	read_size;

	if (!(buf = (char*)malloc(sizeof(char) * BUFFER_SIZE + 1)))
		return(-1);
	if ((read_size = read(fd, buf, BUFFER_SIZE)) == -1)
		return (-1);
	if (read_size < BUFFER_SIZE)
		lst->eof_flag = 1;
	buf[read_size] = '\0';
	eol_flag = 0;
	if((lf = ft_strchr(buf, '\n')))
	{
		eol_flag = 1;
		*lf++ = '\0';
		if (!(lst->exstr = ft_strdup(lf)))
			return (error_processor(line, buf, lst));
	}
	if (!(tmp = ft_strjoin(*line, buf)))
		return (error_processor(line, buf, lst));
	free(*line);
	free(buf);
	*line = tmp;
	if (lst->eof_flag && !(eol_flag))
		return (0);
	return (eol_flag ? 1 : my_read(fd, lst, line));
}

int		error_processor(char **line, char *buf, t_list *lst)
{
	free(*line);
	free(buf);
	free(lst->exstr);
	return (-1);
}
