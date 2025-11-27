/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_block.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 20:50:06 by stales            #+#    #+#             */
/*   Updated: 2025/11/27 21:03:54 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"
#include <immintrin.h>
#include <emmintrin.h>
#include <wmmintrin.h>

__m128i	aes_block_enc(__m128i data, const aes_key_t *k, const aes_key_size_t nr)
{
	if (!k)
		return (_mm_setzero_si128());

    // Xor data with first round Key (This XOR is equal to first AddRounKey Transformation)
	data = AddRoundKey(data, k->sched[0]);

	data = _mm_aesenc_si128(data, k->sched[1]);
	data = _mm_aesenc_si128(data, k->sched[2]);
	data = _mm_aesenc_si128(data, k->sched[3]);
	data = _mm_aesenc_si128(data, k->sched[4]);
	data = _mm_aesenc_si128(data, k->sched[5]);
	data = _mm_aesenc_si128(data, k->sched[6]);
	data = _mm_aesenc_si128(data, k->sched[7]);
	data = _mm_aesenc_si128(data, k->sched[8]);
	data = _mm_aesenc_si128(data, k->sched[9]);

	if (nr >= AES_192_NR) {
		
		data = _mm_aesenc_si128(data, k->sched[10]);
		data = _mm_aesenc_si128(data, k->sched[11]);

		if (nr >= AES_256_NR) {
			data = _mm_aesenc_si128(data, k->sched[12]);
			data = _mm_aesenc_si128(data, k->sched[13]);
		}
	}

    //      a[127:0] := ShiftRows(a[127:0])
	//      a[127:0] := SubBytes(a[127:0])
	//      dst[127:0] := a[127:0] (AddRoundKey) XOR RoundKey[127:0]

	data = _mm_aesenclast_si128(data, k->sched[nr]);

	return (data);
}

__m128i		aes_block_dec(__m128i data, const aes_key_t *k, const aes_key_size_t nr)
{
    if (!k)
        return (_mm_setzero_si128());

    data = AddRoundKey(data, k->sched[nr]);

	data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 1]));
	data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 2]));
	data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 3]));
	data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 4]));
	data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 5]));
	data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 6]));
	data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 7]));
	data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 8]));
	data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 9]));

	if (nr >= AES_192_NR) {
		data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 10]));
		data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 11]));
			
		if (nr == AES_256_NR) {
			data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 12]));
			data = _mm_aesdec_si128(data, _mm_aesimc_si128(k->sched[nr - 13]));
		}
	}
    
    data = _mm_aesdeclast_si128(data, k->sched[0]);

    return (data);
}
