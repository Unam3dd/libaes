#include "pkcs.h"

#include "aes.h"

#include <stdio.h>
#include <string.h>

static void	print_hex(const uint8_t *buf, size_t size)
{
	static const char HEX_TAB[0x10] = {
		'0', '1', '2',
		'3', '4', '5',
		'6', '7', '8',
		'9', 'A', 'B',
		'C', 'D', 'E',
		'F'
	};

	if (!buf) return ;

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
	
	uint8_t in[0x20];
	uint8_t out[0x20];

	memset(in, 0, sizeof(in));
	memset(out, 0, sizeof(out));
	memset(&ctx, 0, sizeof(ctx));

	ctx.key_size = AES_KEY_128;
	ctx.mode = AES_128_ECB;

	strncpy((char *)in, "hello world !!!!", sizeof(in));

	printf("Input buffer = ");
	print_hex(in, sizeof(in));

	memset((char *)ctx.key.key_128, 'A', 0x10);

	printf("Original AES-128-ECB Key : ");
	print_hex(ctx.key.key_128, sizeof(ctx.key.key_128));

	aes_128_key_expansion(&ctx.key);

	aes_128_ecb_enc(out, sizeof(out), in, sizeof(in), &ctx);

	printf("Output buffer after encryption = ");
	print_hex(out, sizeof(out));

	memcpy(in, out, sizeof(in));

	memset(out, 0, sizeof(out));

	aes_128_ecb_dec(out, sizeof(out), in, sizeof(in), &ctx);

	printf("Output buffer after decryption = ");
	print_hex(out, sizeof(out));

	return (0);
}
