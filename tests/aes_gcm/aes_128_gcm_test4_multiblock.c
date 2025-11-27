#include "aes.h"
#include <stdio.h>
#include <string.h>

/*
 * Test: AES-128-GCM with multiple blocks
 * Testing multi-block encryption
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
	
	// Simple test key
	uint8_t key[16];
	memset(key, 0x42, 16);
	
	// Simple nonce
	uint8_t nonce[16];
	memset(nonce, 0, 16);
	nonce[15] = 1;
	
	// AAD: some metadata
	uint8_t aad[] = "Metadata Information";
	
	// Plaintext: 3 blocks (48 bytes)
	uint8_t plaintext[48];
	for (size_t i = 0; i < 48; i++) {
		plaintext[i] = (uint8_t)(i & 0xFF);
	}
	
	uint8_t ciphertext[48] = {0};
	uint8_t decrypted[48] = {0};
	uint8_t tag_enc[16] = {0};
	uint8_t tag_dec[16] = {0};
	
	memset(&ctx, 0, sizeof(ctx));
	ctx.key_size = AES_KEY_128;
	memcpy(ctx.key.key_128, key, 16);
	
	// Key expansion
	if (aes_128_key_expansion(&ctx.key) != AES_OK) {
		fprintf(stderr, "Error: Key expansion failed\n");
		return 1;
	}
	
	printf("=== AES-128-GCM Test 4 (Multi-block) ===\n");
	printf("Key:       ");
	print_hex(key, 16);
	printf("Nonce:     ");
	print_hex(nonce, 16);
	printf("AAD:       %s (%zu bytes)\n", aad, strlen((char*)aad));
	printf("\nPlaintext (%zu bytes):\n", sizeof(plaintext));
	print_hex(plaintext, sizeof(plaintext));
	
	// Encrypt
	gcm_out.out = ciphertext;
	gcm_out.size = sizeof(ciphertext);
	
	if (aes_gcm_enc(&gcm_out, nonce, aad, strlen((char*)aad), plaintext, sizeof(plaintext), &ctx) != AES_OK) {
		fprintf(stderr, "Error: Encryption failed\n");
		return 1;
	}
	
	_mm_storeu_si128((__m128i*)tag_enc, gcm_out.tag);
	
	printf("\nCiphertext (%zu bytes):\n", sizeof(ciphertext));
	print_hex(ciphertext, sizeof(ciphertext));
	printf("\nTag (encryption):\n");
	print_hex(tag_enc, 16);
	
	// Decrypt (using alias function)
	gcm_out.out = decrypted;
	gcm_out.size = sizeof(decrypted);
	
	if (aes_gcm_dec(&gcm_out, nonce, aad, strlen((char*)aad), ciphertext, sizeof(ciphertext), &ctx) != AES_OK) {
		fprintf(stderr, "Error: Decryption failed\n");
		return 1;
	}
	
	_mm_storeu_si128((__m128i*)tag_dec, gcm_out.tag);
	
	printf("\nDecrypted plaintext:\n");
	print_hex(decrypted, sizeof(decrypted));
	printf("\nTag (decryption):\n");
	print_hex(tag_dec, 16);
	
	// Verify decryption matches original plaintext
	if (memcmp(plaintext, decrypted, sizeof(plaintext)) != 0) {
		fprintf(stderr, "\nError: Decrypted plaintext doesn't match original!\n");
		return 1;
	}
	
	// Verify tags match
	if (memcmp(tag_enc, tag_dec, 16) != 0) {
		fprintf(stderr, "\nError: Tags don't match!\n");
		return 1;
	}
	
	printf("\n✓ Test PASSED (encryption/decryption successful)!\n");
	return 0;
}

