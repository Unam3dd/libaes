#include "aes.h"
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
	aes_key_t	keys;

	memset(&keys, 0, sizeof(keys));

	urandom(keys.key_128, sizeof(keys.key_128));

	printf("Original Keys :\n");
	print_hex(keys.key_128, sizeof(keys.key_128));

	aes_128_key_expansion(&keys);

	for (int i = 0; i < AES_128_NR+1; i++) {
		printf("RKEY[%02d] = ", i);
		print_hex((const uint8_t *)&keys.sched_128[i], sizeof(keys.sched_128[i]));
	}

	return (0);
}
