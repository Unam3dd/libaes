#include "rand.h"
#include <stdio.h>
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
	uint8_t	buf[0x20];

	memset(buf, 0, sizeof(buf));

	if (buf != rdrnd(buf, 5))
		return (1);

	print_hex(buf, sizeof(buf));
	
	if (buf != rdrnd(buf, 8))
		return (1);
	
	print_hex(buf, sizeof(buf));

	if (buf != rdrnd(buf, 0x10))
		return (1);
	
	print_hex(buf, sizeof(buf));

	return (0);
}
