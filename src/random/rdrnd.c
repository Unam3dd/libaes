/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rdrnd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 18:21:42 by stales            #+#    #+#             */
/*   Updated: 2024/12/10 20:35:04 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rand.h"
#include <immintrin.h>
#include <stdint.h>

#if defined (__RDRND__)

static inline uint8_t *r(uint8_t *restrict ptr, size_t *size)
{
	uint32_t r = 0;

	if (!(*size & 0x7))
		return (ptr);

	while (*size & 0x7) {
		
		_rdrand32_step(&r);

		if ((*size & 0x7) >= sizeof(uint32_t)) {
			*(uint32_t*)ptr = r;
			ptr += sizeof(uint32_t);
			*size -= sizeof(uint32_t);
			continue ;
		}

		if ((*size & 0x7) >= sizeof(uint16_t)) {
			*(uint16_t*)ptr = r & 0xFFFF;
			ptr += sizeof(uint32_t);
			*size -= sizeof(uint32_t);
			continue ;
		}

		*ptr++ = r & 0xFF;
		(*size)--;
	}

	return (ptr);
}

uint8_t	*rdrnd(uint8_t *restrict buf, size_t size)
{
	if (!buf) return (NULL);

	if (!size) return (buf);

	uint8_t *ptr = NULL;

	ptr = r(buf, &size);

	while (size) {
		_rdrand64_step((unsigned long long *)ptr);
		ptr += 0x8;
		size -= 0x8;
	}

	return (buf);
}

#endif
