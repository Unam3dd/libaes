#include "aes.h"
#include <stdio.h>
#include <string.h>

static void print_m128i(const char *label, __m128i val)
{
	uint8_t buf[16];
	_mm_storeu_si128((__m128i*)buf, val);
	printf("%s", label);
	for (int i = 0; i < 16; i++) {
		printf("%02x", buf[i]);
	}
	printf("\n");
}

int main(void)
{
	aes_ctx_t ctx;
	aes_gcm_counter_t gcm_out;
	
	uint8_t key[16] = {0};
	uint8_t nonce[16] = {0};
	uint8_t *aad = NULL;
	size_t aad_len = 0;
	uint8_t ciphertext[16] = {0};
	uint8_t tag[16] = {0};
	uint8_t dummy_pt[16] = {0};
	
	memset(&ctx, 0, sizeof(ctx));
	ctx.key_size = AES_KEY_128;
	memcpy(ctx.key.key_128, key, 16);
	
	if (aes_128_key_expansion(&ctx.key) != AES_OK) {
		return 1;
	}
	
	gcm_out.out = ciphertext;
	gcm_out.size = 0;
	
	printf("=== Debug Test Case 1 ===\n");
	printf("Key:   00...00\n");
	printf("Nonce: 00...00\n");
	printf("AAD:   (empty)\n");
	printf("PT:    (empty)\n\n");
	
	// Calcul manuel de ce qu'on attend
	// H = E(K=0, 0^128)
	__m128i zero = _mm_setzero_si128();
	__m128i h = _mm_aesenc_si128(zero, ctx.key.sched[0]);
	for (int i = 1; i < 10; i++) {
		h = _mm_aesenc_si128(h, ctx.key.sched[i]);
	}
	h = _mm_aesenclast_si128(h, ctx.key.sched[10]);
	
	print_m128i("H (hash subkey): ", h);
	
	// J0 = 00...01 (compteur = 1 en big-endian)
	uint8_t j0_bytes[16] = {0};
	j0_bytes[15] = 0x01;
	__m128i j0 = _mm_loadu_si128((__m128i*)j0_bytes);
	
	// E(K, J0)
	__m128i j0_enc = _mm_aesenc_si128(j0, ctx.key.sched[0]);
	for (int i = 1; i < 10; i++) {
		j0_enc = _mm_aesenc_si128(j0_enc, ctx.key.sched[i]);
	}
	j0_enc = _mm_aesenclast_si128(j0_enc, ctx.key.sched[10]);
	
	print_m128i("E(K, J0):        ", j0_enc);
	
	printf("\nAppel de aes_gcm_enc...\n");
	if (aes_gcm_enc(&gcm_out, nonce, aad, aad_len, dummy_pt, 0, &ctx) != AES_OK) {
		return 1;
	}
	
	_mm_storeu_si128((__m128i*)tag, gcm_out.tag);
	
	print_m128i("Tag généré:      ", gcm_out.tag);
	
	printf("\nAttendu:         58e2fccefa7e3061367f1d57a4e7455a\n");
	
	return 0;
}
