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
	uint8_t in[0x40];
	uint8_t	cpy[0x40];
	uint8_t out[0x40];

	memset(in, 0, sizeof(in));
	memset(out, 0, sizeof(out));
	memset(&ctx, 0, sizeof(ctx));

	ctx.key_size = AES_KEY_128;
	ctx.pad = TRUE;

	// Test with different pattern
	for (size_t i = 0; i < 0x28; i++)
		in[i] = (uint8_t)((i * 7 + 13) & 0xFF);

	strncpy((char *)cpy, (char *)in, sizeof(cpy));
	
	pkcs_status_t status = PKCS_OK;

	if ((status = pkcs_pad(in, sizeof(in), 0x28)) != PKCS_OK) {
		fprintf(stderr, "Error: pkcs_pad() = %d\n", status);
		return 1;
	}

	printf("=== AES-128-ECB Test 2 ===\n");
	printf("Input buffer (padded) = ");
	print_hex(in, sizeof(in));

	// Different key pattern
	for (size_t i = 0; i < 16; i++)
		ctx.key.key_128[i] = (uint8_t)((i * 11 + 7) & 0xFF);

	printf("AES-128-ECB Key : ");
	print_hex(ctx.key.key_128, sizeof(ctx.key.key_128));

	aes_128_key_expansion(&ctx.key);

	aes_ecb_enc(out, sizeof(out), in, sizeof(in), &ctx);

	printf("Output after encryption = ");
	print_hex(out, sizeof(out));

	memcpy(in, out, sizeof(in));
	memset(out, 0, sizeof(out));

	aes_ecb_dec(out, sizeof(out), in, sizeof(in), &ctx);

	printf("Output after decryption = ");
	print_hex(out, sizeof(out));

	if ((status = pkcs_unpad(out, sizeof(out), 0x10)) != PKCS_OK) {
		fprintf(stderr, "Error: pkcs_unpad() = %d\n", status);
		return 1;
	}

	printf("Output after unpad = ");
	print_hex(out, sizeof(out));

	if (memcmp(cpy, out, sizeof(out))) {
		fprintf(stderr, "Error: Mismatch!\n");
		return 1;
	}

	printf("✓ AES-128-ECB Test 2 PASSED!\n");
	return 0;
}


