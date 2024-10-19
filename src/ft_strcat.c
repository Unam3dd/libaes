/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:54:51 by stales            #+#    #+#             */
/*   Updated: 2022/04/12 18:28:29 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	char	*tmp;
	char	*tsrc;

	tmp = dest;
	tsrc = (char *)src;
	while (*tmp)
		tmp++;
	while (*tsrc)
		*tmp++ = *tsrc++;
	*tmp = 0;
	return (dest);
}
