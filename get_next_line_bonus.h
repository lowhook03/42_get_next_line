/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junlee2 <junlee2@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/28 09:58:22 by junlee2           #+#    #+#             */
/*   Updated: 2022/08/29 10:19:35 by junlee2          ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

// # define BUFFER_SIZE 1

# include <stdlib.h>
# include <unistd.h>

typedef struct s_fdlist
{
	int				fd;
	char			buffer[BUFFER_SIZE];
	char			end_op;
	ssize_t			totallen;
	ssize_t			stack;
	ssize_t			s_idx;
	ssize_t			c_idx;
	ssize_t			status;
	ssize_t			indent;
	struct s_fdlist	*next;
	struct s_fdlist	*prev;
}	t_fdlist;

char		*get_next_line(int fd);
char		*free_fdlist(t_fdlist *fdlist);
void		*ft_memcpy(void *dst, const void *src, size_t n);

#endif