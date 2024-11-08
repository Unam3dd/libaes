/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_ni_128_expansion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:21:57 by stales            #+#    #+#             */
/*   Updated: 2024/11/05 21:09:22 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
 *
 * 				AES-NI Key Expansion Algorithm
 * 				https://en.wikipedia.org/wiki/AES_key_schedule
 *
 *
 *	Intel Intrinsics Guide : https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
 *
 * 	XMM Register is 128 Bits
 * 	YMM Register is 256 Bits
 * 	ZMM Register is 512 Bits
 *
 *	__m128i _mm_setzero_si128(): Is just pxor xmm, xmm where arg1 is XMM register (128 bits).
 *						Return vector of type __m128i with all elements set to zero.
 *						Instruction: pxor xmm, xmm
 *						CPUID Flags: SSE2
 *
 *	__m128i _mm_xor_si128(__m128i a, __m128i b): Compute the bitwise XOR of 128 bits
 *					(representing integer data) in a and b, and store the result in dst.
 *					Instruction: pxor xmm, xmm
 *					CPUID Flags: SSE2
 *
 *
 *	__m128i _mm_slli_si128(__m128i a, __m128i b): Shift left 
 *						by imm8 bytes while shifting in zeros, and store the results in dst.
 *						Instruction: pslldq xmm, imm8
 *						CPUID Flags: SSE2
 *
 *	__m128i _mm_shuffle_epi32 (__m128i a, int imm8): 
 *						Shuffle 32-bit integers in a using the control in imm8, and store the results in dst.
 *						Instruction: pshufd xmm, xmm, imm8
 *						CPUID Flags: SSE2
 *									
 *
 *
 * This Code written with AES-NI from the AES-NI INTEL manual.
 *
 * IF YOU WANT CHECK ALGORITHMS WITHOUT AES-NI you can check aes_no_ni_key_128_expansion.c
 */

#if defined (__AES__) && defined (__SSE2__)

#include  "aes.h"		// For Round Constant

#include <wmmintrin.h>	// AES-NI

#include <emmintrin.h>	// SSE2

static inline aes_round_key_t AES_128_ASSIST(__m128i tmp1, __m128i tmp2)
{
	__m128i tmp3 = _mm_setzero_si128();

	tmp2 = _mm_shuffle_epi32(tmp2, 0xff);
	tmp3 = _mm_slli_si128(tmp1, 0x4);
	tmp1 = _mm_xor_si128(tmp1, tmp3);
	tmp3 = _mm_slli_si128(tmp3, 0x4);
	tmp1 = _mm_xor_si128(tmp1, tmp3);
	tmp3 = _mm_slli_si128(tmp3, 0x4);
	tmp1 = _mm_xor_si128(tmp1, tmp3);
	tmp1 = _mm_xor_si128(tmp1, tmp2);
	
	return (tmp1);
}

static void	AES_NI_128_KEY_EXPANSION(const byte_t *cipher_key, aes_round_key_t *sched)
{

	__m128i			tmp1 = _mm_setzero_si128();
	__m128i			tmp2 = _mm_setzero_si128();

	// Round 0 (First Round use just with AddRoundKeys)

	tmp1 = _mm_load_si128((__m128i *)cipher_key);
	sched[0] = tmp1;

	// Round 1 - RC[1] = 0x1 _mm_aeskeygenassist_si128 want RC constant integer.
	// You can check Round constant algorithm in rc.c
	// https://en.wikipedia.org/wiki/AES_key_schedule

	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x1);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[1] = tmp1;

	// Round 2 - RC[2] = 0x2
	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x2);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[2] = tmp1;

	// Round 3 - RC[3] = 0x4
	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x4);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[3] = tmp1;


	// Round 4 - RC[4] = 0x8
	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x8);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[4] = tmp1;


	// Round 5 - RC[5] = 0x10
	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x10);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[5] = tmp1;


	// Round 6 - RC[6] = 0x20
	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x20);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[6] = tmp1;


	// Round 7 - RC[7] = 0x40
	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x40);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[7] = tmp1;


	// Round 8 - RC[8] = 0x80
	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x80);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[8] = tmp1;


	// Round 9 - RC[9] = 0x1b
	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x1b);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[9] = tmp1;

	// Round 10 - RC[10] = 0x36
	tmp2 = _mm_aeskeygenassist_si128(tmp1, 0x36);
	tmp1 = AES_128_ASSIST(tmp1, tmp2);
	sched[10] = tmp1;
}

#else
static void	AES_NI_128_KEY_EXPANSION(const byte_t *cipher_key, aes_round_key_t *sched)
{
	(void)cipher_key, (void)sched;
	return ;
}
#endif
