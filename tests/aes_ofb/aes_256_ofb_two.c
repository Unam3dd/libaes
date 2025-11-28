#include "pkcs.h"
#include "aes.h"

#include <stdio.h>
#include <string.h>

static void	print_hex(const uint8_t *buf, size_t size)
{
	static const char HEX_TAB[0x10] = {
		'0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};

	if (!buf) return;

	while (size > 0) {
		putchar(HEX_TAB[*buf >> 0x4 & 0xF]);
		putchar(HEX_TAB[*buf++ & 0xF]);
		putchar(' ');
		size--;
	}
	putchar('\n');
}

int main(void)
{
	aes_ctx_t ctx;
	iv_t iv;
	iv_t iv_copy;
	
	uint8_t in[0x50];
	uint8_t	cpy[0x50];
	uint8_t out[0x50];

	memset(in, 0, sizeof(in));
	memset(out, 0, sizeof(out));
	memset(&ctx, 0, sizeof(ctx));

	ctx.key_size = AES_KEY_256;
	ctx.pad = TRUE;

	// Pattern fill
	for (size_t i = 0; i < 0x3A; i++)
		in[i] = (uint8_t)((i * 3 + 7) & 0xFF);

	strncpy((char *)cpy, (char *)in, sizeof(cpy));
	
	pkcs_status_t status = PKCS_OK;

	if ((status = pkcs_pad(in, sizeof(in), 0x3A)) != PKCS_OK) {
		fprintf(stderr, "Error: pkcs_pad() = %d\n", status);
		return 1;
	}

	printf("=== AES-256-OFB Test 2 ===\n");

	// 256-bit key
	for (size_t i = 0; i < 32; i++)
		ctx.key.key_256[i] = (uint8_t)((i * 7 + 23) & 0xFF);

	// IV
	memset(iv, 0, sizeof(iv_t));
	memcpy(iv, "OFB-256-TEST-IV", 15);
	memcpy(iv_copy, iv, sizeof(iv_t));

	printf("AES-256-OFB Key (first 16 bytes): ");
	print_hex(ctx.key.key_256, 16);
	printf("IV : ");
	print_hex(iv, sizeof(iv_t));

	aes_256_key_expansion(&ctx.key);

	aes_ofb_enc(out, sizeof(out), iv, in, sizeof(in), &ctx);

	printf("Ciphertext (first 32 bytes) = ");
	print_hex(out, 32);

	memcpy(in, out, sizeof(in));
	memset(out, 0, sizeof(out));
	memcpy(iv, iv_copy, sizeof(iv_t));

	aes_ofb_dec(out, sizeof(out), iv, in, sizeof(in), &ctx);

	if ((status = pkcs_unpad(out, sizeof(out), 0x10)) != PKCS_OK) {
		fprintf(stderr, "Error: pkcs_unpad() = %d\n", status);
		return 1;
	}

	if (memcmp(cpy, out, sizeof(out))) {
		fprintf(stderr, "Error: Decryption mismatch!\n");
		return 1;
	}

	printf("✓ AES-256-OFB Test 2 PASSED!\n");
	return 0;
}


