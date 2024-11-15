/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buf.h                                        |    |  |   |   |     |_    */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 17:46:02 by stales            #+#    #+#             */
/*   Updated: 2024/10/25 17:40:49 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUF_H
#define BUF_H

#include <stdint.h>
#include <stddef.h>

#ifndef IS_NOT_ALIGNED
#define IS_NOT_ALIGNED(sz, blk) (sz & ~-blk)
#endif

#ifndef ALIGN
#define ALIGN(sz, blk) ((sz + blk) & ~(~-blk))
#endif

#ifndef __SSE2__

#include <string.h>
#define ZERO_MEMORY(ptr, size) memset(ptr, 0, size)

#else

#include <emmintrin.h>

static void	*zero_memory(const void *ptr, size_t size)
{
	if (!ptr || IS_NOT_ALIGNED(size, 16))
		return (NULL);
	
	__m128i	*tmp = (__m128i*)ptr;
	
	while (size) {
		*tmp++ = _mm_setzero_si128();
		size -= 0x10;
	}

	return ((void *)ptr);
}

#endif

#ifndef BUF
#define BUF(T, S, N) 				\
	typedef struct buf_##N { 		\
		T buf[S];					\
		union {						\
			unsigned char	*str;	\
			uint64_t		*u64;	\
			uint32_t		*u32;	\
			uint16_t		*u16;	\
			uint8_t			*u8;	\
		};							\
		union {						\
			size_t	size;			\
			size_t	length;			\
		};							\
	} buf_##N;
#endif

#ifndef BUF_NEW
#define BUF_NEW(N, name)			\
	buf_##N name;					\
	name.u8 = (byte_t*)name.buf;	\
	name.size = sizeof(name.buf)

#endif

#endif
