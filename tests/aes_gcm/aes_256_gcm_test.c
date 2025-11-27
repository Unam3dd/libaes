#include "aes.h"
#include <stdio.h>
#include <string.h>

/*
 * Test: AES-256-GCM
 * Testing with AES-256 key size
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
	
	// 256-bit key
	uint8_t key[32];
	for (size_t i = 0; i < 32; i++) {
		key[i] = (uint8_t)((i * 7 + 13) & 0xFF);
	}
	
	uint8_t nonce[16];
	memset(nonce, 0, 16);
	memcpy(nonce, "AES-256-NONCE", 13);
	
	uint8_t aad[] = "Top Secret Metadata for AES-256-GCM";
	
	uint8_t plaintext[64];
	for (size_t i = 0; i < 64; i++) {
		plaintext[i] = (uint8_t)((i ^ 0xAA) & 0xFF);
	}
	
	uint8_t ciphertext[64] = {0};
	uint8_t decrypted[64] = {0};
	uint8_t tag_enc[16] = {0};
	uint8_t tag_dec[16] = {0};
	
	memset(&ctx, 0, sizeof(ctx));
	ctx.key_size = AES_KEY_256;
	memcpy(ctx.key.key_256, key, 32);
	
	if (aes_256_key_expansion(&ctx.key) != AES_OK) {
		fprintf(stderr, "Error: AES-256 key expansion failed\n");
		return 1;
	}
	
	printf("=== AES-256-GCM Test ===\n");
	printf("Key (256-bit):\n");
	print_hex(key, 32);
	printf("\nNonce:\n");
	print_hex(nonce, 16);
	printf("\nAAD: '%s'\n\n", aad);
	
	// Encrypt
	gcm_out.out = ciphertext;
	gcm_out.size = sizeof(ciphertext);
	
	if (aes_gcm_enc(&gcm_out, nonce, aad, strlen((char*)aad), plaintext, sizeof(plaintext), &ctx) != AES_OK) {
		fprintf(stderr, "Error: Encryption failed\n");
		return 1;
	}
	
	_mm_storeu_si128((__m128i*)tag_enc, gcm_out.tag);
	
	printf("Plaintext (%zu bytes):\n", sizeof(plaintext));
	print_hex(plaintext, sizeof(plaintext));
	printf("\nCiphertext (%zu bytes):\n", sizeof(ciphertext));
	print_hex(ciphertext, sizeof(ciphertext));
	printf("\nAuthentication Tag:\n");
	print_hex(tag_enc, 16);
	
	// Decrypt
	gcm_out.out = decrypted;
	gcm_out.size = sizeof(decrypted);
	
	if (aes_gcm_dec(&gcm_out, nonce, aad, strlen((char*)aad), ciphertext, sizeof(ciphertext), &ctx) != AES_OK) {
		fprintf(stderr, "Error: Decryption failed\n");
		return 1;
	}
	
	_mm_storeu_si128((__m128i*)tag_dec, gcm_out.tag);
	
	printf("\nDecrypted plaintext:\n");
	print_hex(decrypted, sizeof(decrypted));
	
	// Verify
	if (memcmp(plaintext, decrypted, sizeof(plaintext)) != 0) {
		fprintf(stderr, "\nError: Decryption mismatch!\n");
		return 1;
	}
	
	if (memcmp(tag_enc, tag_dec, 16) != 0) {
		fprintf(stderr, "\nError: Tags don't match!\n");
		return 1;
	}
	
	printf("\n✓ AES-256-GCM Test PASSED!\n");
	return 0;
}

