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
	aes_ctx_t		ctx;
	iv_t			cnt;
	
	uint8_t in[0x40];
	uint8_t cpy[0x40];
	uint8_t out[0x40];

	memset(in, 0, sizeof(in));
	memset(out, 0, sizeof(out));
	memset(&ctx, 0, sizeof(ctx));
	memset(&cnt, 0, sizeof(cnt));

	ctx.key_size = AES_KEY_192;

	strncpy((char *)in, "hello world !!!!", sizeof(in));
	strncpy((char *)cpy, (char *)in, sizeof(cpy));

	printf("Input buffer = ");
	print_hex(in, sizeof(in));

	memset(ctx.key.key_192, 'A', 0x18);
	memset(cnt, 0, sizeof(cnt));
    memset(cnt, 0x41, 0xC);

	printf("Original AES-192-CTR Key : ");
	print_hex(ctx.key.key_192, sizeof(ctx.key.key_192));

	aes_192_key_expansion(&ctx.key);

	aes_ctr_enc(out, sizeof(out), cnt, in, sizeof(in), &ctx);

	printf("Output buffer after encryption = ");
	print_hex(out, sizeof(out));

	memcpy(in, out, sizeof(in));

	memset(out, 0, sizeof(out));

	aes_ctr_dec(out, sizeof(out), cnt, in, sizeof(in), &ctx);

	printf("Output buffer after decryption = ");
	print_hex(out, sizeof(out));

	if (memcmp(out, cpy, sizeof(cpy)))
		return (1);

	return (0);
}
