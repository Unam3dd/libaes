#include "pkcs.h"
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
	char	buf[0x10];
	pkcs_status_t status = 0;

	memset(buf, 0, sizeof(buf));

	strncpy(buf, "hello", sizeof(buf));

	puts("SHOULD FAIL !");

	if ((status = pkcs_pad((uint8_t *)buf, sizeof(buf), 0x20)) != PKCS_OK) {
		print_hex((uint8_t *)buf, sizeof(buf));
		fprintf(stderr, "pkcs_pad() : %d\n", status);
		return (0);
	}
	

	return (1);
}
