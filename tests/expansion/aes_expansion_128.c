#include "aes.h"
#include "rand.h"

#include <immintrin.h>
#include <stdio.h>
#include <string.h>

static void	print_hex_bytes(const uint8_t *buf, size_t size)
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
	aes_key_t	keys;
	memset(&keys, 0, sizeof(keys));
	urandom(keys.key_128, sizeof(keys.key_128));
	size_t i = 0;

    printf("Number of keys : %zd\n", sizeof(keys.sched_128) / sizeof(aes_round_key_t));
	printf("Original Keys :\n");
	print_hex_bytes(keys.key_128, sizeof(keys.key_128));

	aes_128_key_expansion(&keys);

	__asm__ volatile ("xor rbx, rbx\n\t");

	while (i < 10)
		printf("%ld\n", i++);

	return (0);
}
