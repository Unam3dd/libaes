#include "aes.h"
#include <stdio.h>
#include <string.h>

/*
 * Test: AES-128-GCM with partial block
 * Testing non-aligned data (not multiple of 16 bytes)
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
	
	uint8_t key[16];
	memset(key, 0xAB, 16);
	
	uint8_t nonce[16];
	memset(nonce, 0, 16);
	nonce[11] = 0xCA;
	nonce[15] = 0x01;
	
	// AAD with non-block-aligned size
	uint8_t aad[] = "Short AAD";
	
	// Plaintext: 23 bytes (not a multiple of 16)
	uint8_t plaintext[] = "This is a test message!";
	size_t pt_len = strlen((char*)plaintext);
	
	uint8_t ciphertext[32] = {0}; // Need space for padded block
	uint8_t decrypted[32] = {0};
	uint8_t tag_enc[16] = {0};
	uint8_t tag_dec[16] = {0};
	
	memset(&ctx, 0, sizeof(ctx));
	ctx.key_size = AES_KEY_128;
	memcpy(ctx.key.key_128, key, 16);
	
	if (aes_128_key_expansion(&ctx.key) != AES_OK) {
		fprintf(stderr, "Error: Key expansion failed\n");
		return 1;
	}
	
	printf("=== AES-128-GCM Test 5 (Partial Block) ===\n");
	printf("Key:       ");
	print_hex(key, 16);
	printf("Nonce:     ");
	print_hex(nonce, 16);
	printf("AAD:       '%s' (%zu bytes)\n", aad, strlen((char*)aad));
	printf("Plaintext: '%s' (%zu bytes)\n\n", plaintext, pt_len);
	
	// Encrypt
	gcm_out.out = ciphertext;
	gcm_out.size = sizeof(ciphertext);
	
	if (aes_gcm_enc(&gcm_out, nonce, aad, strlen((char*)aad), plaintext, pt_len, &ctx) != AES_OK) {
		fprintf(stderr, "Error: Encryption failed\n");
		return 1;
	}
	
	_mm_storeu_si128((__m128i*)tag_enc, gcm_out.tag);
	
	printf("Ciphertext (%zu bytes):\n", pt_len);
	print_hex(ciphertext, pt_len);
	printf("\nTag (encryption):\n");
	print_hex(tag_enc, 16);
	
	// Decrypt
	gcm_out.out = decrypted;
	gcm_out.size = sizeof(decrypted);
	
	if (aes_gcm_dec(&gcm_out, nonce, aad, strlen((char*)aad), ciphertext, pt_len, &ctx) != AES_OK) {
		fprintf(stderr, "Error: Decryption failed\n");
		return 1;
	}
	
	_mm_storeu_si128((__m128i*)tag_dec, gcm_out.tag);
	
	printf("\nDecrypted plaintext:\n");
	print_hex(decrypted, pt_len);
	printf("As string: '%.*s'\n", (int)pt_len, decrypted);
	printf("\nTag (decryption):\n");
	print_hex(tag_dec, 16);
	
	// Verify
	if (memcmp(plaintext, decrypted, pt_len) != 0) {
		fprintf(stderr, "\nError: Decrypted plaintext doesn't match!\n");
		return 1;
	}
	
	if (memcmp(tag_enc, tag_dec, 16) != 0) {
		fprintf(stderr, "\nError: Tags don't match!\n");
		return 1;
	}
	
	printf("\n✓ Test PASSED!\n");
	return 0;
}


