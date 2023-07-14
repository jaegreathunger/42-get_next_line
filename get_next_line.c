/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaeshin <jaeshin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/12 16:02:38 by jaeshin           #+#    #+#             */
/*   Updated: 2023/07/14 10:42:02 by jaeshin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*join_free(char *buffer, char *temp)
{
	char	*joined;

	joined = ft_strjoin(buffer, temp);
	free(buffer);
	return (joined);
}

char	*get_chunk_strings(int fd, char *buffer)
{
	char	*temp;
	int		read_size;

	// First buffer is empty.
	if (!buffer)
	buffer = ft_calloc(1, sizeof(char));
	temp = (char *)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	read_size = 1;
	while (read_size > 0)
	{
		// reads the file using size given.
		read_size = read(fd, temp, BUFFER_SIZE);
		if (read_size == -1)
		{
			free(temp);
			return (NULL);
		}
		temp[read_size] = '\0';
		buffer = join_free(buffer, temp);
		// if it contains a new line, then break.
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(temp);
	return (buffer);
}

char	*get_return_line(char *chunk)
{
	char	*result;
	size_t	i;

	i = 0;
	if (!chunk[i])
		return (NULL);
	// size of line upto new line.
	while (chunk[i] && chunk[i] != '\n')
		i++;
	// allocates the memory based on the size.
	result = (char *)ft_calloc(i + 2, sizeof(char));
	i = 0;
	// copies the value.
	while (chunk[i] && chunk[i] != '\n')
	{
		result[i] = chunk[i];
		i++;
	}
	if (chunk[i] && chunk[i] == '\n')
		result[i] = '\n';
	result[i + 1] = '\0';
	return (result);
}

char	*get_ready_for_next(char *chunk)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	//size upto new line.
	while (chunk[i] && chunk[i] != '\n')
		i++;
	if (!chunk[i])
	{
		free(chunk);
		return (NULL);
	}
	//allocates the size and copies the rest of the chunk.
	result = (char *)ft_calloc(ft_strlen(chunk) - i + 1, sizeof(char));
	i++;
	while (chunk[i])
		result[j++] = chunk[i++];
	result[j] = '\0';
	free(chunk);
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	//gets a chunk of strings that contains new line.
	buffer = get_chunk_strings(fd, buffer);
	//gets a line including the new line from the chunk.
	line = get_return_line(buffer);
	//saves the rest of the chunk excluding the line to be returned.
	buffer = get_ready_for_next(buffer);
	return (line);
}
