/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoami <tkoami@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 18:12:39 by tkoami            #+#    #+#             */
/*   Updated: 2021/01/18 13:10:59 by tkoami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2
# endif
# define D_SUCCESS 1
# define D_ERROR -1
# define D_EOF 0

typedef struct	s_list
{
	int				fd;
	char			*exstr;
	struct s_list	*prev;
	struct s_list	*next;
}				t_list;

int				get_next_line(int fd, char **line);
size_t			ft_strlen(const char *s);
char			*ft_strchr(const char *s, int c);
char			*ft_strdup(const char *s1);
char			*ft_strjoin(char *s1, char *s2);
t_list			*get_list(int fd, t_list *lst);
t_list			*list_init(int fd);
int				my_read(int fd, char **buf, t_list *lst, char **line);
void			safe_free(char **s);
int				error_processor(char **line, char *buf, t_list **lst, int res);

#endif
