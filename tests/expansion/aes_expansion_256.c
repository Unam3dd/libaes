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
	__m128i zero = _mm_setzero_si128();
	
    memset(&keys, 0, sizeof(keys));
	
    urandom(keys.key_256, sizeof(keys.key_256));

	printf("%zd\n", sizeof(keys.sched_256));

    printf("Number of keys : %zd\n", sizeof(keys.sched_256) / sizeof(aes_round_key_t));
	
    aes_256_key_expansion(&keys);

	for (size_t i = 0; i < sizeof(keys.buf_sched_256) / AES_KEY_ROUND_SIZE; i++) {
		printf("AES-256 ROUND KEY[%ld] = ", i);
		if (!memcmp((__m128i*)&keys.round_keys[i], &zero, AES_KEY_ROUND_SIZE))
			return (1);
		print_hex((const uint8_t *)&keys.round_keys[i], AES_KEY_ROUND_SIZE);
	}

	return (0);
}
