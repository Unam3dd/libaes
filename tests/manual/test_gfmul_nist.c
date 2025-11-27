#include "gf.h"
#include <stdio.h>
#include <string.h>
#include <immintrin.h>

static void print_hex(const char *label, const uint8_t *buf, size_t size)
{
	printf("%s", label);
	for (size_t i = 0; i < size; i++) {
		printf("%02x", buf[i]);
	}
	printf("\n");
}

int main(void)
{
	printf("=== Test gfmul avec vecteurs NIST GCM ===\n\n");
	
	// H from NIST test case 4 (déjà calculé comme E(K, 0^128))
	uint8_t h_original[16] = {
		0xb8, 0x3b, 0x53, 0x37, 0x08, 0xbf, 0x53, 0x5d,
		0x0a, 0xa6, 0xe5, 0x29, 0x80, 0xd5, 0x3b, 0x78
	};
	
	// Byte-swap H (comme fait dans compute_ghash)
	const __m128i BSWAP_MASK = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	__m128i h_m128 = _mm_loadu_si128((__m128i*)h_original);
	h_m128 = _mm_shuffle_epi8(h_m128, BSWAP_MASK);
	uint8_t h[16];
	_mm_storeu_si128((__m128i*)h, h_m128);
	
	print_hex("H original:      ", h_original, 16);
	print_hex("H byte-swapped:  ", h, 16);
	printf("\n");
	
	// Test 1: Calculer ghash pour le premier bloc AAD
	uint8_t aad1_original[16] = {
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef
	};
	
	// Byte-swap AAD1
	__m128i aad1_m128 = _mm_loadu_si128((__m128i*)aad1_original);
	aad1_m128 = _mm_shuffle_epi8(aad1_m128, BSWAP_MASK);
	uint8_t aad1[16];
	_mm_storeu_si128((__m128i*)aad1, aad1_m128);
	
	printf("Test 1: AAD[0] * H\n");
	print_hex("AAD[0] original:   ", aad1_original, 16);
	print_hex("AAD[0] swapped:    ", aad1, 16);
	
	uint8_t result[16];
	
	// ghash = (0 XOR AAD[0]) * H = AAD[0] * H
	gfmul(aad1, h, result);
	
	print_hex("Result gfmul:      ", result, 16);
	
	// Résultat attendu (calculé avec une implémentation de référence)
	// Pour GCM, après AAD[0] * H (avec les bytes swappés correctement)
	// le résultat devrait être un certain vecteur
	
	printf("\n");
	
	// Test 2: Vérifier avec un calcul simple
	uint8_t zero[16] = {0};
	uint8_t result_zero[16];
	
	printf("Test 2: 0 * H = 0\n");
	gfmul(zero, h, result_zero);
	print_hex("Result:            ", result_zero, 16);
	
	if (memcmp(result_zero, zero, 16) == 0) {
		printf("✅ Test 0*H PASSED\n");
	} else {
		printf("❌ Test 0*H FAILED\n");
	}
	
	return 0;
}

