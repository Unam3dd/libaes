/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   urandom.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 18:14:52 by stales            #+#    #+#             */
/*   Updated: 2024/11/12 22:10:06 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rand.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

uint8_t	*urandom(uint8_t *restrict buf, size_t size)
{
	if (!buf) return (NULL);

	if (!size) return (buf);

	int fd = open("/dev/urandom", O_RDONLY);

	if (fd < 0) return (NULL);

	if (read(fd, buf, size) < 0) {
		perror("read");
		close(fd);
		return (NULL);
	}

	close(fd);

	return (buf);
}
