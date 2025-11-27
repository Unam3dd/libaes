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
	iv_t cnt;
	
	uint8_t in[0x40];
	uint8_t	cpy[0x40];
	uint8_t out[0x40];

	memset(in, 0, sizeof(in));
	memset(out, 0, sizeof(out));
	memset(&ctx, 0, sizeof(ctx));

	ctx.key_size = AES_KEY_256;

	memset(in, 0, sizeof(in));
	memset(in, 0x42, 0x25); // 37 bytes

	strncpy((char *)cpy, (char *)in, sizeof(cpy));
	
	printf("=== AES-256-CTR Test ===\n");
	printf("Input buffer = ");
	print_hex(in, sizeof(in));

	// 256-bit key
	for (size_t i = 0; i < 32; i++)
		ctx.key.key_256[i] = (uint8_t)(i * 3 & 0xFF);

	memset(cnt, 0, sizeof(iv_t));
	memcpy(cnt, "CTR-NONCE-256", 13);

	printf("AES-256-CTR Key : ");
	print_hex(ctx.key.key_256, sizeof(ctx.key.key_256));

	aes_256_key_expansion(&ctx.key);

	aes_ctr_enc(out, sizeof(out), cnt, in, sizeof(in), &ctx);

	printf("Output after encryption = ");
	print_hex(out, sizeof(out));

	memcpy(in, out, sizeof(in));
	memset(out, 0, sizeof(out));

	aes_ctr_dec(out, sizeof(out), cnt, in, sizeof(in), &ctx);

	printf("Output after decryption = ");
	print_hex(out, sizeof(out));

	if (memcmp(cpy, out, sizeof(out))) {
		fprintf(stderr, "Error: Decryption doesn't match original!\n");
		return 1;
	}

	printf("✓ AES-256-CTR Test PASSED!\n");
	return 0;
}
