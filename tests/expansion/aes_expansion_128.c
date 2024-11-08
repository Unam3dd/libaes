#include "aes.h"
#include "rand.h"

#include <immintrin.h>
#include <emmintrin.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

	printf("%zd\n", sizeof(keys.sched_128));

    printf("Number of keys : %zd\n", sizeof(keys.sched_128) / sizeof(aes_round_key_t));
	
    aes_128_key_expansion(&keys);

	printf("%p\n", (void *)&keys.round_keys[2]);

	for (size_t i = 0; i < sizeof(keys.buf_sched_128) / AES_KEY_128; i++) {
		printf("AES-128 ROUND KEY[%ld] = ", i);
		print_hex((const uint8_t *)(void *)&keys.round_keys[i], AES_KEY_128);
		//print_hex((const uint8_t *)(keys.buf_sched_128 + (i * AES_KEY_128)), AES_KEY_128);
	}

	return (0);
}
