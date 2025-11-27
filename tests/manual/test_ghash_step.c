#include "aes.h"
#include "gf.h"
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
	
	// Key from NIST test
	uint8_t key[16] = {
		0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
		0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08
	};
	
	memset(&ctx, 0, sizeof(ctx));
	ctx.key_size = AES_KEY_128;
	memcpy(ctx.key.key_128, key, 16);
	
	if (aes_128_key_expansion(&ctx.key) != AES_OK) {
		fprintf(stderr, "Error: Key expansion failed\n");
		return 1;
	}
	
	printf("=== Test GHASH Step by Step ===\n\n");
	
	// Step 1: Calculate H = E(K, 0^128)
	__m128i zero = _mm_setzero_si128();
	__m128i h = zero;
	
	h = _mm_xor_si128(h, ctx.key.sched[0]);
	h = _mm_aesenc_si128(h, ctx.key.sched[1]);
	h = _mm_aesenc_si128(h, ctx.key.sched[2]);
	h = _mm_aesenc_si128(h, ctx.key.sched[3]);
	h = _mm_aesenc_si128(h, ctx.key.sched[4]);
	h = _mm_aesenc_si128(h, ctx.key.sched[5]);
	h = _mm_aesenc_si128(h, ctx.key.sched[6]);
	h = _mm_aesenc_si128(h, ctx.key.sched[7]);
	h = _mm_aesenc_si128(h, ctx.key.sched[8]);
	h = _mm_aesenc_si128(h, ctx.key.sched[9]);
	h = _mm_aesenclast_si128(h, ctx.key.sched[10]);
	
	print_m128i("H (hash subkey): ", h);
	
	// Expected H for this key: b83b533708bf535d0aa6e52980d53b78
	uint8_t expected_h[16] = {
		0xb8, 0x3b, 0x53, 0x37, 0x08, 0xbf, 0x53, 0x5d,
		0x0a, 0xa6, 0xe5, 0x29, 0x80, 0xd5, 0x3b, 0x78
	};
	
	printf("H attendu:       ");
	for (int i = 0; i < 16; i++) printf("%02x", expected_h[i]);
	printf("\n\n");
	
	uint8_t h_bytes[16];
	_mm_storeu_si128((__m128i*)h_bytes, h);
	
	if (memcmp(h_bytes, expected_h, 16) == 0) {
		printf("✅ Hash subkey H est CORRECT!\n");
	} else {
		printf("❌ Hash subkey H est INCORRECT!\n");
		printf("Différences:\n");
		for (int i = 0; i < 16; i++) {
			if (h_bytes[i] != expected_h[i]) {
				printf("  Byte %d: calculé=0x%02x, attendu=0x%02x\n",
				       i, h_bytes[i], expected_h[i]);
			}
		}
	}
	
	return 0;
}

