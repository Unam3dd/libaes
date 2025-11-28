/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_gcm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 12:46:51 by stales            #+#    #+#             */
/*   Updated: 2025/11/28 19:14:37 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"
#include "gf.h"
#include <string.h>

#include <emmintrin.h>
#include <smmintrin.h>
#include <wmmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>

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
			hash_subkey = _mm_aesenc_si128(hash_subkey, ctx->key.sched[0xd]);
		}
	}

	hash_subkey = _mm_aesenclast_si128(hash_subkey, ctx->key.sched[NR]);

	return (hash_subkey);
}

/////////////////////////////////////
//
//
//		Compute GHASH
//
//
////////////////////////////////////

static __m128i	compute_ghash(const __m128i hash_subkey, const byte_t *restrict aad, size_t aad_len, const byte_t *restrict ciphertext, size_t ct_len)
{
	__m128i ghash = _mm_setzero_si128();
	__m128i temp = _mm_setzero_si128();
	uint64_t aad_bits = 0, ct_bits = 0;
	size_t i = 0, aad_blocks = 0, aad_remainder = 0, ct_blocks = 0, ct_remainder = 0;
	byte_t partial_aad[16] = {0}, partial_ct[16] = {0}, lengths[16] = {0};

	// Traiter les blocs complets de AAD
	if (aad && aad_len > 0) {

		aad_blocks = aad_len >> 4;
		
		for (i = 0; i < aad_blocks; i++) {
			temp = _mm_loadu_si128(&((__m128i*)aad)[i]);
			ghash = _mm_xor_si128(ghash, temp);
			gfmul((uint8_t*)&ghash, (uint8_t*)&hash_subkey, (uint8_t*)&ghash);
		}

		// Traiter le dernier bloc partiel de AAD s'il existe
		aad_remainder = aad_len & 0xF;
		if (aad_remainder) {
			for (i = 0; i < aad_remainder; i++)
				partial_aad[i] = aad[aad_blocks * 16 + i];
			temp = _mm_loadu_si128((__m128i*)partial_aad);
			ghash = _mm_xor_si128(ghash, temp);
			gfmul((uint8_t*)&ghash, (uint8_t*)&hash_subkey, (uint8_t*)&ghash);
		}
	}

	// Traiter les blocs complets de ciphertext
	ct_blocks = ct_len >> 4;
	for (i = 0; i < ct_blocks; i++) {
		temp = _mm_loadu_si128(&((__m128i*)ciphertext)[i]);
		ghash = _mm_xor_si128(ghash, temp);
		gfmul((uint8_t*)&ghash, (uint8_t*)&hash_subkey, (uint8_t*)&ghash);
	}

	// Traiter le dernier bloc partiel de ciphertext s'il existe
	ct_remainder = ct_len & 0xF;
	if (ct_remainder) {
		for (i = 0; i < ct_remainder; i++)
			partial_ct[i] = ciphertext[ct_blocks * 16 + i];
		temp = _mm_loadu_si128((__m128i*)partial_ct);
		ghash = _mm_xor_si128(ghash, temp);
		gfmul((uint8_t*)&ghash, (uint8_t*)&hash_subkey, (uint8_t*)&ghash);
	}

	// Ajouter les longueurs (len(A) || len(C)) en bits
	aad_bits = aad_len * 8;
	ct_bits = ct_len * 8;
	
	// Big-endian encoding
	for (i = 0; i < 8; i++) {
		lengths[7 - i] = (aad_bits >> (i * 8)) & 0xFF;
		lengths[15 - i] = (ct_bits >> (i * 8)) & 0xFF;
	}
	
	temp = _mm_loadu_si128((__m128i*)lengths);
	ghash = _mm_xor_si128(ghash, temp);
	gfmul((uint8_t*)&ghash, (uint8_t*)&hash_subkey, (uint8_t*)&ghash);

	return (ghash);
}

/////////////////////////////////////
//
//
//	    AES Galois Counter Mode
//
//
////////////////////////////////////

static aes_status_t aes_gcm_crypt(aes_gcm_counter_t *out, const iv_t nonce, const byte_t *restrict aad, size_t aad_len, const byte_t *restrict in, size_t i_sz, const aes_ctx_t *ctx, bool_t is_decrypt)
{
	if (!ctx || !out || !in || !out->out || (out->size < i_sz))
		return (AES_ERR);

	__m128i state = _mm_setzero_si128();
	__m128i feedback = _mm_setzero_si128();
	__m128i j0_encrypted = _mm_setzero_si128();
	__m128i hash_subkey = _mm_setzero_si128();
	__m128i ghash = _mm_setzero_si128();
	iv_t nonce_local = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint32_t *cnt = NULL;
	size_t NR = 0, blocks = 0, i = 0;

	// Copie locale du nonce pour pouvoir incrémenter le compteur
	memcpy(nonce_local, nonce, 16);

	cnt = (uint32_t *)(nonce_local + 0xC);

	NR = (ctx->key_size == AES_KEY_128
		? AES_128_NR 
		: ctx->key_size == AES_KEY_192 
		? AES_192_NR
		: AES_256_NR);

	hash_subkey = create_hash_subkey(NR, ctx);

	// Calculer E(K, J0) pour le tag
	feedback = _mm_loadu_si128((__m128i*)nonce_local);
	j0_encrypted = aes_block_enc(feedback, &ctx->key, NR);
	
	*cnt += 0x01000000;

	// Chiffrer/Déchiffrer les données avec J1, J2, J3, ...
	blocks = (i_sz & 0xF ?  -~(i_sz >> 0x4) : (i_sz >> 0x4));

	for (i = 0; i < blocks; i++) {

		// Prefetching
		_mm_prefetch((__m128i*)(in + 0x20), _MM_HINT_T0);

		state = _mm_loadu_si128( &((__m128i*)in)[i]);

		// Load current counter (J1, J2, J3, ...)
		feedback = _mm_loadu_si128((__m128i*)nonce_local);

		feedback = aes_block_enc(feedback, &ctx->key, NR);

		state = _mm_xor_si128(feedback, state);

		_mm_storeu_si128(&((__m128i*)out->out)[i], state);
		
		// Incrémenter pour le prochain bloc (big endian)
		*cnt += 0x01000000;
	}

	// Calculer GHASH sur le ciphertext (toujours)
	// Pour encryption: ciphertext = out->out (résultat du CTR)
	// Pour decryption: ciphertext = in (entrée)
	ghash = compute_ghash(hash_subkey, aad, aad_len, is_decrypt ? in : out->out, i_sz);
	
	// Tag final = GHASH XOR E(K, J0)
	out->tag = _mm_xor_si128(ghash, j0_encrypted);

	return (AES_OK);
}

aes_status_t	aes_gcm_enc(aes_gcm_counter_t *out, const iv_t nonce, const byte_t *restrict aad, size_t aad_len, const byte_t *restrict in, size_t i_sz, const aes_ctx_t *ctx)
{
	return (aes_gcm_crypt(out, nonce, aad, aad_len, in, i_sz, ctx, FALSE));
}

aes_status_t	aes_gcm_dec(aes_gcm_counter_t *out, const iv_t nonce, const byte_t *restrict aad, size_t aad_len, const byte_t *restrict in, size_t i_sz, const aes_ctx_t *ctx)
{
	return (aes_gcm_crypt(out, nonce, aad, aad_len, in, i_sz, ctx, TRUE));
}

