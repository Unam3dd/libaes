/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_words.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/12 18:50:33 by stales            #+#    #+#             */
/*   Updated: 2022/04/12 18:50:47 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_get_words(char const *str, char c)
{
	char	*tstr;
	int		count;

	tstr = (char *)str;
	count = 0;
	while (*tstr)
	{
		while (*tstr && *tstr == c)
			tstr++;
		if (*tstr && *tstr != c)
			count++;
		while (*tstr && *tstr != c)
			tstr++;
	}
	return (count);
}
