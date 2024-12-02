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
	
	uint8_t in[0x100];
	uint8_t cpy[0x100];
	uint8_t out[0x100];

	memset(in, 0, sizeof(in));
	memset(out, 0, sizeof(out));
	memset(&ctx, 0, sizeof(ctx));

	ctx.key_size = AES_KEY_128;

	strncpy((char *)in, "hello world this is a simple very simple basic message !!!!", sizeof(in));

	strncpy((char *)cpy, (char *)in, sizeof(cpy));
	
    pkcs_status_t status = PKCS_OK;

	if ((status = pkcs_pad(in, sizeof(in), 0x10)) != PKCS_OK) {
		fprintf(stderr, "Error: pkcs_pad() = %d\n", status);
		return (1);
	}

	printf("Input buffer = ");
	print_hex(in, sizeof(in));

	memset((char *)ctx.key.key_128, 'A', 0x10);
    memset(ctx.iv, 0x41, 0x10);

	printf("Original AES-128-OFB Key : ");
	print_hex(ctx.key.key_128, sizeof(ctx.key.key_128));

    printf("IV AES-128-OFB: ");
    print_hex(ctx.iv, sizeof(ctx.iv));

	aes_128_key_expansion(&ctx.key);

	aes_ofb_enc(out, sizeof(out), ctx.iv, in, sizeof(in), &ctx);

	printf("Output buffer after encryption = ");
	print_hex(out, sizeof(out));

	memcpy(in, out, sizeof(in));

	memset(out, 0, sizeof(out));

	aes_ofb_dec(out, sizeof(out), ctx.iv, in, sizeof(in), &ctx);

	printf("Output buffer after decryption = ");
	print_hex(out, sizeof(out));

	if ((status = pkcs_unpad(out, sizeof(out), 0x10)) != PKCS_OK) {
		fprintf(stderr, "Error: pkcs_unpad() = %d\n", status);
		return (1);
	}

	printf("Output buffer after unpad = ");
	print_hex(out, sizeof(out));

	if (memcmp(out, cpy, sizeof(out)))
		return (1);

	return (0);
}

