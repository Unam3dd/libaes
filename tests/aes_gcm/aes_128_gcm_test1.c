#include "aes.h"
#include <stdio.h>
#include <string.h>

/*
 * Test Vector from NIST SP 800-38D
 * Test Case 1: AES-128-GCM
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
	
	// Test Case 1 from NIST
	// Key: 00000000000000000000000000000000
	uint8_t key[16] = {0};
	
	// IV/Nonce: 000000000000000000000000 (96 bits)
	// J0 = IV || 0x00000001 (big-endian counter)
	uint8_t nonce[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
	};
	
	// No AAD
	uint8_t *aad = NULL;
	size_t aad_len = 0;
	
	// No plaintext (empty for test case 1)
	(void)aad;  // Suppress unused warning
	
	// Expected tag: 58e2fccefa7e3061367f1d57a4e7455a
	uint8_t expected_tag[16] = {
		0x58, 0xe2, 0xfc, 0xce, 0xfa, 0x7e, 0x30, 0x61,
		0x36, 0x7f, 0x1d, 0x57, 0xa4, 0xe7, 0x45, 0x5a
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
	
	printf("=== AES-128-GCM Test 1 (NIST Test Vector) ===\n");
	printf("Key:       ");
	print_hex(key, 16);
	printf("Nonce:     ");
	print_hex(nonce, 16);
	printf("AAD:       (empty)\n");
	printf("Plaintext: (empty)\n\n");
	
	// Encrypt (with empty plaintext, we just generate the tag)
	// We need at least a minimal plaintext for this implementation
	uint8_t dummy_pt[16] = {0};
	gcm_out.out = ciphertext;
	gcm_out.size = 0; // No actual ciphertext expected
	
	if (aes_gcm_enc(&gcm_out, nonce, aad, aad_len, dummy_pt, 0, &ctx) != AES_OK) {
		fprintf(stderr, "Error: Encryption failed\n");
		return 1;
	}
	
	// Extract tag
	_mm_storeu_si128((__m128i*)tag, gcm_out.tag);
	
	printf("Generated Tag:  ");
	print_hex(tag, 16);
	printf("Expected Tag:   ");
	print_hex(expected_tag, 16);
	
	// Compare tag
	if (memcmp(tag, expected_tag, 16) != 0) {
		fprintf(stderr, "\nError: Tag mismatch!\n");
		return 1;
	}
	
	printf("\n✓ Test PASSED!\n");
	return 0;
}

