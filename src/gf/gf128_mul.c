/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gf128_mul.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 23:35:39 by stales            #+#    #+#             */
/*   Updated: 2024/12/19 13:23:33 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gf.h"

#include <emmintrin.h>  // SSE2 pour __m128i
#include <wmmintrin.h>  // Pour PCLMULQDQ (Carry-less Multiplication)
#include <stdint.h>
#include <smmintrin.h>

// Polynôme irréductible pour GF(2^128) : x^128 + x^7 + x^2 + x + 1

__m128i gf128_mul(__m128i a, __m128i b) {
    
	const __m128i reduction_poly = _mm_set_epi32(0, 0, 0, REDUCTION_POLY_128);
    
	// Carry-less multiplication with PCLMULQDQ
    __m128i product_high = _mm_clmulepi64_si128(a, b, 0x11);  // High Part
    __m128i product_low  = _mm_clmulepi64_si128(a, b, 0x00);  // Low Part
    __m128i product_mid1 = _mm_clmulepi64_si128(a, b, 0x01);  // Mid Part
    __m128i product_mid2 = _mm_clmulepi64_si128(a, b, 0x10);  // Mid Part

    __m128i mid = _mm_xor_si128(product_mid1, product_mid2);
    mid = _mm_xor_si128(mid, _mm_srli_si128(mid, 8));
    __m128i result = _mm_xor_si128(product_low, _mm_slli_si128(mid, 8));
    result = _mm_xor_si128(result, _mm_slli_si128(product_high, 8));

    for (int i = 0; i < 8; ++i) {
        
		__m128i carry = _mm_and_si128(result, _mm_set_epi32(0x80000000, 0, 0, 0));
        
		result = _mm_slli_epi64(result, 1);
        
		if (!_mm_testz_si128(carry, carry))
            result = _mm_xor_si128(result, reduction_poly);
    }

    return result;
}

