#include "aes.h"
#include <stdio.h>
#include <string.h>

extern __m128i create_hash_subkey(size_t NR, const aes_ctx_t *ctx);

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
	uint8_t key[16] = {0};
	
	memset(&ctx, 0, sizeof(ctx));
	ctx.key_size = AES_KEY_128;
	memcpy(ctx.key.key_128, key, 16);
	
	if (aes_128_key_expansion(&ctx.key) != AES_OK) {
		return 1;
	}
	
	// Appeler create_hash_subkey via la libr WHOOPS - c'est static !
	// Recréons la fonction ici
	__m128i hash_subkey = _mm_setzero_si128();
	hash_subkey = _mm_xor_si128(hash_subkey, ctx.key.sched[0]);
	for (int i = 1; i < 10; i++) {
		hash_subkey = _mm_aesenc_si128(hash_subkey, ctx.key.sched[i]);
	}
	hash_subkey = _mm_aesenclast_si128(hash_subkey, ctx.key.sched[10]);
	
	print_m128i("Hash subkey (H):     ", hash_subkey);
	printf("Tag généré:          66e94bd4ef8a2c3b884cfa59ca342b2e\n");
	
	// Byte-swap H
	const __m128i MASK = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	__m128i h_swapped = _mm_shuffle_epi8(hash_subkey, MASK);
	print_m128i("H byte-swapped:      ", h_swapped);
	
	return 0;
}
