#include "aes.h"
#include <stdio.h>
#include <string.h>

/*
 * Test Vector from NIST SP 800-38D
 * Test Case 2: AES-128-GCM with plaintext
 */

static void	print_hex(const uint8_t *buf, size_t size)
{
	static const char HEX_TAB[0x10] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

	if (!buf) return;

	for (size_t i = 0; i < size; i++) {
		putchar(HEX_TAB[buf[i] >> 4]);
		putchar(HEX_TAB[buf[i] & 0xF]);
		if ((i + 1) % 16 == 0 && i != size - 1)
			putchar('\n');
		else if (i != size - 1)
			putchar(' ');
	}
	putchar('\n');
}

int main(void)
{
	aes_ctx_t ctx;
	aes_gcm_counter_t gcm_out;
	
	// Key: 00000000000000000000000000000000
	uint8_t key[16] = {0};
	
	// IV: 000000000000000000000000 (96 bits)
	// J0 = IV || 0x00000001 (big-endian counter)
	uint8_t nonce[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
	};
	
	// Plaintext: 00000000000000000000000000000000
	uint8_t plaintext[16] = {0};
	
	// Expected ciphertext: 0388dace60b6a392f328c2b971b2fe78
	uint8_t expected_ct[16] = {
		0x03, 0x88, 0xda, 0xce, 0x60, 0xb6, 0xa3, 0x92,
		0xf3, 0x28, 0xc2, 0xb9, 0x71, 0xb2, 0xfe, 0x78
	};
	
	// Expected tag: ab6e47d42cec13bdf53a67b21257bddf
	uint8_t expected_tag[16] = {
		0xab, 0x6e, 0x47, 0xd4, 0x2c, 0xec, 0x13, 0xbd,
		0xf5, 0x3a, 0x67, 0xb2, 0x12, 0x57, 0xbd, 0xdf
	};
	
	uint8_t ciphertext[16] = {0};
	uint8_t tag[16] = {0};
	
	memset(&ctx, 0, sizeof(ctx));
	ctx.key_size = AES_KEY_128;
	memcpy(ctx.key.key_128, key, 16);
	
	// Key expansion
	if (aes_128_key_expansion(&ctx.key) != AES_OK) {
		fprintf(stderr, "Error: Key expansion failed\n");
		return 1;
	}
	
	gcm_out.out = ciphertext;
	gcm_out.size = sizeof(ciphertext);
	
	printf("=== AES-128-GCM Test 2 (NIST Test Vector with Plaintext) ===\n");
	printf("Key:       ");
	print_hex(key, 16);
	printf("Nonce:     ");
	print_hex(nonce, 16);
	printf("Plaintext: ");
	print_hex(plaintext, 16);
	printf("\n");
	
	// Encrypt
	if (aes_gcm_enc(&gcm_out, nonce, NULL, 0, plaintext, 16, &ctx) != AES_OK) {
		fprintf(stderr, "Error: Encryption failed\n");
		return 1;
	}
	
	// Extract tag
	_mm_storeu_si128((__m128i*)tag, gcm_out.tag);
	
	printf("Generated Ciphertext: ");
	print_hex(ciphertext, 16);
	printf("Expected Ciphertext:  ");
	print_hex(expected_ct, 16);
	printf("\nGenerated Tag:  ");
	print_hex(tag, 16);
	printf("Expected Tag:   ");
	print_hex(expected_tag, 16);
	
	// Compare ciphertext
	if (memcmp(ciphertext, expected_ct, 16) != 0) {
		fprintf(stderr, "\nError: Ciphertext mismatch!\n");
		return 1;
	}
	
	// Compare tag
	if (memcmp(tag, expected_tag, 16) != 0) {
		fprintf(stderr, "\nError: Tag mismatch!\n");
		return 1;
	}
	
	printf("\n✓ Test PASSED!\n");
	return 0;
}

