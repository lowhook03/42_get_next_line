/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junlee2 <junlee2@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 09:51:24 by junlee2           #+#    #+#             */
/*   Updated: 2022/08/29 10:17:41 by junlee2          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#include <fcntl.h>
#include <stdio.h>

t_fdlist	*find_fd(int fd, t_fdlist *fdlist)
{
	t_fdlist	*fdnode;

	fdnode = fdlist;
	while (fdnode->next != 0)
	{
		if (fdnode->next->fd == fd)
			return (fdnode->next);
		fdnode = fdnode->next;
	}
	fdnode->next = (t_fdlist *)malloc(sizeof(t_fdlist));
	if (!fdnode->next)
		return (0);
	fdnode->next->fd = fd;
	fdnode->next->s_idx = -1;
	fdnode->next->next = 0;
	fdnode->next->prev = fdnode;
	return (fdnode->next);
}

void	return_buffer(t_fdlist buff, char *returnstr, ssize_t max_idx)
{
	ssize_t	idx;
	ssize_t	bsize;

	if (!returnstr)
		return ;
	bsize = BUFFER_SIZE;
	idx = buff.s_idx;
	while (idx < max_idx)
	{
		returnstr[buff.stack * bsize + idx - buff.indent] = buff.buffer[idx];
		idx++;
	}
}

char	*get_line_re(t_fdlist	*fdlist)
{
	char		*returnstr;
	t_fdlist	buff;

	if (fdlist->status == 0)
	{
		if (fdlist->totallen == 0)
			return (0);
		return ((char *)malloc(fdlist->totallen + 1));
	}
	ft_memcpy(&buff, fdlist, sizeof(t_fdlist));
	while (buff.c_idx < buff.status)
	{
		fdlist->totallen++;
		if (buff.buffer[buff.c_idx] == '\n')
		{
			returnstr = (char *)malloc(fdlist->totallen + 1);
			return_buffer(buff, returnstr, buff.c_idx + 1);
			fdlist->s_idx = buff.c_idx + 1;
			return (returnstr);
		}
		buff.c_idx++;
	}
	fdlist->s_idx = 0;
	fdlist->c_idx = 0;
	fdlist->status = read(fdlist->fd, fdlist->buffer, BUFFER_SIZE);
	if (fdlist->status == -1)
		return (0);
	fdlist->stack++;
	returnstr = get_line_re(fdlist);
	return_buffer(buff, returnstr, buff.c_idx);
	return (returnstr);
}

char	*line_manager(t_fdlist	*fdlist)
{
	char	*returnstr;

	fdlist->totallen = 0;
	if (fdlist->s_idx == -1)
	{
		fdlist->status = read(fdlist->fd, fdlist->buffer, BUFFER_SIZE);
		if (fdlist->status == 0 || fdlist->status == -1)
			return (free_fdlist(fdlist));
		fdlist->s_idx = 0;
	}
	fdlist->indent = fdlist->s_idx;
	fdlist->c_idx = fdlist->s_idx;
	fdlist->stack = 0;
	returnstr = get_line_re(fdlist);
	if (returnstr == 0)
		return(free_fdlist(fdlist));
	if (fdlist->status == 0)
		free_fdlist(fdlist);
	returnstr[fdlist->totallen] = 0;
	return (returnstr);
}

char	*get_next_line(int fd)
{
	static t_fdlist	*fdhead = 0;
	t_fdlist		*workbench;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	if (!fdhead)
	{
		fdhead = (t_fdlist *)malloc(sizeof(t_fdlist));
		if (!fdhead)
			return (0);
		fdhead->fd = -1;
		fdhead->next = 0;
	}
	workbench = find_fd(fd, fdhead);
	if (!workbench)
		return (0);
	return (line_manager(workbench));
}

// int	main(void)
// {
// 	int		fd;

// 	fd = open("a.txt", O_RDONLY);
// 	while (printf("%s", get_next_line(fd)) != 6){}
// }