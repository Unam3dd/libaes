/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_gcm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 12:46:51 by stales            #+#    #+#             */
/*   Updated: 2025/02/08 16:20:25 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"
#include "gf.h"

#include <emmintrin.h>
#include <smmintrin.h>
#include <wmmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>

/**
* @WARNING: the implementation of GCM is not finish please take care of this
* and don't use it.
*
*/


/////////////////////////////////////
//
//
//		Create Hash SubKey
//
//
////////////////////////////////////

static __m128i	create_hash_subkey(size_t NR, const aes_ctx_t *ctx)
{
	__m128i	hash_subkey = _mm_setzero_si128();

	// Xor State with first round Key (This XOR is equal to first AddRounKey Transformation)
	hash_subkey = AddRoundKey(hash_subkey, ctx->key.sched[0x0]);
        
	hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0x1]);
	hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0x2]);
	hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0x3]);
	hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0x4]);
	hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0x5]);
	hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0x6]);
	hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0x7]);
	hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0x8]);
	hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0x9]);

	if (NR >= AES_192_NR) {
		hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0xa]);
		hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0xb]);

		if (NR == AES_256_NR) {
			hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0xc]);
			hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0xe]);
		}
	}

	hash_subkey = _mm_aesenclast_si128(hash_subkey, ctx->key.sched[NR]);

	return (hash_subkey);
}

/////////////////////////////////////
//
//
//	    AES Galois Counter Mode
//
//
////////////////////////////////////

aes_status_t __attribute__((alias("aes_gcm_enc"))) aes_gcm_dec(aes_gcm_counter_t *out, const iv_t nonce, const byte_t *restrict aad, const byte_t *restrict in, size_t i_sz, const aes_ctx_t *ctx);


aes_status_t	aes_gcm_enc(aes_gcm_counter_t *out, const iv_t nonce, const byte_t *restrict aad, const byte_t *restrict in, size_t i_sz, const aes_ctx_t *ctx)
{
	if (!ctx || !out || !in || !aad || !out->out || (out->size < i_sz))
		return (AES_ERR);

	__m128i state = _mm_setzero_si128();
	__m128i feedback = _mm_setzero_si128();
	__m128i first = _mm_setzero_si128();
	__m128i hash_subkey = _mm_setzero_si128();
	//__m128i current_aad = _mm_loadu_si128(aad);

	uint32_t *cnt = (uint32_t *)(nonce + 0xC);
	uint32_t save = *cnt;

	size_t NR = (ctx->key_size == AES_KEY_128
		? AES_128_NR 
		: ctx->key_size == AES_KEY_192 
		? AES_192_NR
		: AES_256_NR);

	hash_subkey = create_hash_subkey(NR, ctx);

	// How Many iterations of 16 bytes Blocks which represent the number of state ?
	size_t blocks = (i_sz & 0xF ?  -~(i_sz >> 0x4) : (i_sz >> 0x4));

	for (size_t i = 0; i < blocks; i++) {

		// Prefetching
		_mm_prefetch((__m128i*)(in + 0x20), _MM_HINT_T0);

		// Load State
		state = _mm_loadu_si128( &((__m128i*)in)[i]);

		feedback = _mm_loadu_si128((__m128i*)nonce);

		// Xor State with first round Key (This XOR is equal to first AddRounKey Transformation)
		feedback = AddRoundKey(feedback, ctx->key.sched[0x0]);
        
		feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0x1]);
		feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0x2]);
		feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0x3]);
		feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0x4]);
		feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0x5]);
		feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0x6]);
		feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0x7]);
		feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0x8]);
		feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0x9]);

		if (NR >= AES_192_NR) {
			feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0xa]);
			feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0xb]);

			if (NR == AES_256_NR) {
				feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0xc]);
				feedback = _mm_aesenc_si128(feedback, ctx->key.sched[0xe]);
			}
		}

        feedback = _mm_aesenclast_si128(feedback, ctx->key.sched[NR]);
		
		state = _mm_xor_si128(feedback, state);
		
		_mm_storeu_si128(&((__m128i*)out->out)[i], state);

		if (!i) first = feedback;
		
		*cnt += 0x01000000;
	}

	out->tag = _mm_xor_si128(first, hash_subkey);
	
	*cnt = save;

	return (AES_OK);
}

