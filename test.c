#include <stdio.h>
#include "inc/pkcs.h"
#include <string.h>

static void	print_hex(const uint8_t *buf, size_t size)
{
	static const char HEX_TAB[] = {
		'0', '1', '2',
		'3', '4', '5',
		'6', '7', '8',
		'9', 'A', 'B',
		'C', 'D', 'E',
		'F'
	};

	if (!buf) return ;

	while (size) {
		putchar(HEX_TAB[*buf >> 0x4 & 0xF]);
		putchar(HEX_TAB[*buf++ & 0xF]);
		putchar(' ');
		size--;
	}

	putchar('\n');

}

int main(void)
{
	char buf[0x100];

	memset(buf, 0, sizeof(buf));

	strncpy(buf, "hello", sizeof(buf));

	pkcs_pad((uint8_t *)buf, sizeof(buf), 0x10);

	print_hex((uint8_t *)buf, sizeof(buf));

	printf("%d\n", pkcs_unpad((uint8_t *)buf, sizeof(buf), 0x10));

	print_hex((uint8_t *)buf, sizeof(buf));

	return (0);
}
