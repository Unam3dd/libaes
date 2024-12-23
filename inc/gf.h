/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gf.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:31:32 by stales            #+#    #+#             */
/*   Updated: 2024/12/22 22:49:12 by stales           ###   ########.fr       */
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

// gf8_add.c

#ifndef GF8_ADD
#define GF8_ADD(a, b) (a ^ b)
#undef GF8_ADD
#define GF8_ADD(a, b) (a ^ b)
#endif

// gf8_mul.c

uint16_t		gf8_mul(uint8_t a, uint8_t b);

__m128i			gf128_mul(__m128i a, __m128i b);

#endif
