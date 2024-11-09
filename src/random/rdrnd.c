/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rdrnd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 18:21:42 by stales            #+#    #+#             */
/*   Updated: 2024/11/09 09:17:36 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rand.h"
#include <immintrin.h>
#include <stdint.h>

#if defined (__RDRND__)

uint8_t	*rdrnd(uint8_t *restrict buf, size_t size)
{
	if (!buf) return (NULL);

	if (!size) return (buf);

	uint8_t *ptr = buf;
	uint16_t r = 0;

	while (size & 0x7) {
		
		_rdrand16_step(&r);

		if ((size & 0x7) >= sizeof(uint16_t)) {
			*(uint16_t*)ptr = r;
			size -= 2;
			ptr += 2;
			continue ;
		}

		*ptr++ = r & 0xFF;
		size--;
	}

	while (size > 0) {
		_rdrand64_step((unsigned long long *)ptr);
		ptr += sizeof(unsigned long long);
		size -= sizeof(unsigned long long);
	}

	return (buf);
}

#endif
