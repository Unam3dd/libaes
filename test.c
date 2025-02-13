#include <emmintrin.h>
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>
#include "inc/gf.h"

void	generate_table_256(void)
{
	int i = 0, j = 0;
	uint32_t	reversed = 0;

	for (i = 0; i < 0x10000; i++) {

		reversed = 0;

		for (j = 0; j < 16; j++) {
			if ((i >> j) & 1)
				reversed |= 1 << (15 - j);
		}

		printf("%d: %x\n", i, reversed);
	}
}

int main(void)
{
	__m128i_u a = _mm_setr_epi32(0xAAAAAAAA, 0xAAAAAAAA, 0, 0);
	__m128i_u b = _mm_setr_epi32(0xBBBBBBBB, 0xBBBBBBBB, 0, 0);

	__m128i_u res = gf128_mul(a, b);

	if (res[0])
		printf("%lx", (unsigned long)res[0]);

	if (res[1])
		printf("%lx", (unsigned long)res[1]);

	return (0);
}
