/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoami <tkoami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 18:12:39 by tkoami            #+#    #+#             */
/*   Updated: 2020/12/08 15:43:54 by tkoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>

# define BUFFER_SIZE 100

typedef struct	s_list
{
	int				fd;
	char			*exstr;
	int				eof_flag;
	struct s_list	*prev;
	struct s_list	*next;
}				t_list;

size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, const char *s2);
t_list	*get_list(int fd, t_list **lst);
t_list	*list_init(int fd);
int		my_read(int fd, t_list *lst, char **line);
#endif