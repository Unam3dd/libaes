/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:56:52 by stales            #+#    #+#             */
/*   Updated: 2022/04/12 14:48:39 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	char	*tmp;
	char	*ptr;

	tmp = (char *)s;
	while (*tmp)
		tmp++;
	ptr = (char *)malloc((tmp - s) + 1);
	tmp = ptr;
	while (*s && tmp)
		*tmp++ = *s++;
	if (ptr)
		*tmp = 0;
	return (ptr);
}
