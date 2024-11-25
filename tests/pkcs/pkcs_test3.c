#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pkcs.h"

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

static size_t len_bytes(const uint8_t *bytes)
{
	if (!bytes) return (0);

	size_t i = 0;
	
	while (*bytes++)
		i++;

	return (i);
}

int main(void)
{
	uint8_t	buf[0x40] = "passwordpassworda";
	uint8_t	cmp[0x40];
	size_t	sz = 0;
	pkcs_status_t	status = 0;
	uint8_t	pad = 0;

	sz = strlen((char *)buf);

	memset(cmp, 0, sizeof(cmp));
	
	pad = PKCS_PAD_LEN(16, sz);
	
	memset(cmp, pad, pad);

	printf("Before padding : ");
	print_hex(buf, sizeof(buf));

	if (pkcs_pad(buf, sizeof(buf), sz, 16) != PKCS_OK)
		return (1);

	printf("After padding : ");
	print_hex(buf, sizeof(buf));

	printf("%zd\n", len_bytes(buf));

	if ((status = pkcs_unpad(buf, sizeof(buf), len_bytes(buf), 16)) != PKCS_OK) {
		fprintf(stderr, "pkcs_unpad() : %d\n", status);
		return (1);
	}

	printf("After Unpad : ");
	print_hex(buf, sizeof(buf));

	if (memcmp((char *)buf, "passwordpassworda", sizeof("passwordpassworda")))
		return (1);

	return (0);
}
