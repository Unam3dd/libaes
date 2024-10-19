/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 18:57:04 by stales            #+#    #+#             */
/*   Updated: 2022/04/12 17:47:46 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*ptr;
	int		i;

	if (!s || !f)
		return (NULL);
	i = 0;
	ptr = (char *)malloc(ft_strlen((char *)s) + 1);
	while (ptr && s[i])
	{
		ptr[i] = f(i, s[i]);
		i++;
	}
	if (ptr)
		ptr[i] = 0;
	return (ptr);
}
