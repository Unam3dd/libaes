#include "aes.h"
#include <stdio.h>
#include <string.h>

/*
 * Test: AES-192-GCM
 * Testing with AES-192 key size
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
	
	// 192-bit key
	uint8_t key[24];
	for (size_t i = 0; i < 24; i++) {
		key[i] = (uint8_t)((i * 17) & 0xFF);
	}
	
	uint8_t nonce[16];
	memset(nonce, 0, 16);
	memcpy(nonce, "AES-192-NONCE", 13);
	
	uint8_t aad[] = "AES-192-GCM Test";
	
	uint8_t plaintext[32];
	memset(plaintext, 0x55, 32);
	
	uint8_t ciphertext[32] = {0};
	uint8_t decrypted[32] = {0};
	uint8_t tag_enc[16] = {0};
	uint8_t tag_dec[16] = {0};
	
	memset(&ctx, 0, sizeof(ctx));
	ctx.key_size = AES_KEY_192;
	memcpy(ctx.key.key_192, key, 24);
	
	if (aes_192_key_expansion(&ctx.key) != AES_OK) {
		fprintf(stderr, "Error: AES-192 key expansion failed\n");
		return 1;
	}
	
	printf("=== AES-192-GCM Test ===\n");
	printf("Key (192-bit):\n");
	print_hex(key, 24);
	printf("Nonce:     ");
	print_hex(nonce, 16);
	printf("AAD:       '%s'\n\n", aad);
	
	// Encrypt
	gcm_out.out = ciphertext;
	gcm_out.size = sizeof(ciphertext);
	
	if (aes_gcm_enc(&gcm_out, nonce, aad, strlen((char*)aad), plaintext, sizeof(plaintext), &ctx) != AES_OK) {
		fprintf(stderr, "Error: Encryption failed\n");
		return 1;
	}
	
	_mm_storeu_si128((__m128i*)tag_enc, gcm_out.tag);
	
	printf("Plaintext:\n");
	print_hex(plaintext, sizeof(plaintext));
	printf("\nCiphertext:\n");
	print_hex(ciphertext, sizeof(ciphertext));
	printf("\nTag:\n");
	print_hex(tag_enc, 16);
	
	// Decrypt
	gcm_out.out = decrypted;
	gcm_out.size = sizeof(decrypted);
	
	if (aes_gcm_dec(&gcm_out, nonce, aad, strlen((char*)aad), ciphertext, sizeof(ciphertext), &ctx) != AES_OK) {
		fprintf(stderr, "Error: Decryption failed\n");
		return 1;
	}
	
	_mm_storeu_si128((__m128i*)tag_dec, gcm_out.tag);
	
	// Verify
	if (memcmp(plaintext, decrypted, sizeof(plaintext)) != 0) {
		fprintf(stderr, "\nError: Decryption mismatch!\n");
		return 1;
	}
	
	if (memcmp(tag_enc, tag_dec, 16) != 0) {
		fprintf(stderr, "\nError: Tags don't match!\n");
		return 1;
	}
	
	printf("\n✓ AES-192-GCM Test PASSED!\n");
	return 0;
}


