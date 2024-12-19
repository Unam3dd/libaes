/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gf.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:31:32 by stales            #+#    #+#             */
/*   Updated: 2024/12/19 13:18:53 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GF_H
#define GF_H

#include <stdint.h>
#include <emmintrin.h>

/////////////////////////////////////
//
//
//				DEFINES
//
//
////////////////////////////////////


#ifndef REDUCTION_POLY_128
#define REDUCTION_POLY_128 0x87
#endif

// gf8_add.c

#ifndef GF8_ADD
#define GF8_ADD(a, b) (a ^ b)
#undef GF8_ADD
#define GF8_ADD(a, b) (a ^ b)
#endif

// gf8_mul.c

uint8_t	gf8_mul(uint8_t a, uint8_t b);

// gf128_mul.c

__m128i_u	gf128_mul(__m128i_u a, __m128i_u b);

#endif
