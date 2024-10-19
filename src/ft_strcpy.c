/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:55:37 by stales            #+#    #+#             */
/*   Updated: 2022/04/12 17:16:55 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	char	*tmp;
	char	*tsrc;

	tmp = dest;
	tsrc = (char *)src;
	while (*tsrc)
		*tmp++ = *tsrc++;
	*tmp = 0;
	return (dest);
}
