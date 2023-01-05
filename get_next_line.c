/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jooheekim <jooheekim@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/16 15:26:50 by joohekim          #+#    #+#             */
/*   Updated: 2023/01/06 04:39:09 by jooheekim        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"

static int	check_nl(char *backup)
{
	int	i;

	i = 0;
	while (backup[i])
	{
		if (backup[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static char	*read_line(int fd, char **backup, int *cnt, char **buf)
{
	char	*temp;

	*buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (*buf == NULL)
		return (NULL);
	(*buf)[0] = '\0';
	while (1)
	{
		*cnt = read(fd, *buf, BUFFER_SIZE);
		if (*cnt == 0)
			break ;
		if (*cnt == -1)
			return (NULL);
		(*buf)[*cnt] = '\0';
		temp = ft_strjoin(*backup, *buf);
		if (temp == NULL)
			return (NULL);
		free(*backup);
		*backup = temp;
		if (check_nl(*backup) != -1)
			return (*backup);
	}
	if ((*backup)[0] == '\0')
		return (NULL);
	return (*backup);
}

static char	*substr_line(char **backup, int idx)
{
	char	*line;
	char	*temp;

	if (idx < 0)
		return (*backup);
	line = ft_substr(*backup, 0, idx + 1);
	if (line == NULL)
		return (NULL);
	temp = ft_substr(*backup, idx + 1, ft_strlen(*backup));
	if (temp == NULL)
	{
		null_free(&line);
		return (NULL);
	}
	free(*backup);
	*backup = ft_strdup(temp);
	if (*backup == NULL)
	{
		null_free(&line);
		null_free(&temp);
		return (NULL);
	}
	free(temp);
	return (line);
}

static char	*return_line(char **backup, int *cnt)
{
	char	*line;
	int		i;

	i = check_nl(*backup);
	line = substr_line(backup, i);
	if (line == NULL)
	{
		null_free(backup);
		return (NULL);
	}
	if (*cnt == 0 && i == -1)
	{
		line = ft_strdup(line);
		if (line == NULL)
		{
			null_free(backup);
			return (NULL);
		}
		null_free(backup);
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*backup;
	char		*line;
	int			cnt;
	char		*buf;

	cnt = 1;
	if (fd < 0)
		return (NULL);
	if (backup == NULL)
	{
		backup = ft_strdup("");
		if (backup == NULL)
			return (NULL);
	}
	if (!read_line(fd, &backup, &cnt, &buf))
	{
		null_free(&backup);
		null_free(&buf);
		return (NULL);
	}
	null_free(&buf);
	line = return_line(&backup, &cnt);
	if (line == NULL)
		return (NULL);
	return (line);
}
