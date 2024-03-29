/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csimon <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/04 10:54:49 by csimon            #+#    #+#             */
/*   Updated: 2017/02/22 12:04:49 by csimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char		*ft_read_line(const int fd, int *ret)
{
	char	buf[BUFF_SIZE + 1];

	*ret = read(fd, buf, BUFF_SIZE);
	buf[*ret] = '\0';
	return (ft_strdup(buf));
}

char		*ft_join(char *s1, char *s2)
{
	char			*str;

	str = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (str);
}

int			ft_feedline(t_list1 *elem, char **line)
{
	int				ret;
	char			*tmp;
	char			*str;

	ret = 1;
	str = (elem->save) ? elem->save : ft_read_line(elem->fd, &ret);
	if (ret <= 0)
		return (ret);
	*line = ft_strdup("\0");
	while (ret > 0)
	{
		if ((tmp = ft_strchr(str, '\n')))
		{
			*tmp = '\0';
			elem->save = ft_strdup(tmp + 1);
			*line = ft_join(*line, str);
			return (1);
		}
		*line = ft_join(*line, str);
		str = ft_read_line(elem->fd, &ret);
	}
	ft_strclr(elem->save);
	if (ft_strlen(*line) != 0)
		return (1);
	return (ret);
}

t_list1		*add_new_elem(const int fd)
{
	t_list1		*elem;

	elem = (t_list1*)malloc(sizeof(t_list1));
	elem->fd = fd;
	elem->next = NULL;
	elem->save = NULL;
	return (elem);
}

int			get_next_line(const int fd, char **line)
{
	int				result;
	static	t_list1	*begin = NULL;
	t_list1			*elem;
	char			buf[BUFF_SIZE];

	if (BUFF_SIZE < 1 || line == NULL || fd < 0 || (read(fd, buf, 0)) < 0)
		return (-1);
	if (begin != NULL)
	{
		elem = begin;
		while (fd != elem->fd && elem->next != NULL)
			elem = elem->next;
		if (fd != elem->fd)
		{
			elem->next = add_new_elem(fd);
			elem = elem->next;
		}
	}
	else
	{
		begin = add_new_elem(fd);
		elem = begin;
	}
	result = ft_feedline(elem, line);
	return (result);
}
