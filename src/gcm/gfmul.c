#include <emmintrin.h>
#include <immintrin.h>
#include <stdint.h>
#include <tmmintrin.h>
#include <xmmintrin.h>

void	gfmul(const uint8_t a[0x10], const uint8_t b[0x10], uint8_t r[0x10])
{
	const __m128i BSWAP_MASK = _mm_set_epi8(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	
	__m128i xmm0 = _mm_loadu_si128((const __m128i*)a);
	__m128i xmm1 = _mm_loadu_si128((const __m128i*)b);
	__m128i tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9;
	
	// GCM byte-swap avant multiplication
	xmm0 = _mm_shuffle_epi8(xmm0, BSWAP_MASK);
	xmm1 = _mm_shuffle_epi8(xmm1, BSWAP_MASK);

	tmp3 = _mm_clmulepi64_si128(xmm0, xmm1, 0x00);
	tmp4 = _mm_clmulepi64_si128(xmm0, xmm1, 0x10);
	tmp5 = _mm_clmulepi64_si128(xmm0, xmm1, 0x01);
	tmp6 = _mm_clmulepi64_si128(xmm0, xmm1, 0x11);

	tmp4 = _mm_xor_si128(tmp4, tmp5);
	tmp5 = _mm_slli_si128(tmp4, 8);
	tmp4 = _mm_srli_si128(tmp4, 8);
	tmp3 = _mm_xor_si128(tmp3, tmp5);
	tmp6 = _mm_xor_si128(tmp6, tmp4);

	tmp7 = _mm_srli_epi32(tmp3, 31);
	tmp8 = _mm_srli_epi32(tmp6, 31);
	tmp3 = _mm_slli_epi32(tmp3, 1);
	tmp6 = _mm_slli_epi32(tmp6, 1);

	tmp9 = _mm_srli_si128(tmp7, 12);
	tmp8 = _mm_slli_si128(tmp8, 4);
	tmp7 = _mm_slli_si128(tmp7, 4);
	tmp3 = _mm_or_si128(tmp3, tmp7);
	tmp6 = _mm_or_si128(tmp6, tmp8);
	tmp6 = _mm_or_si128(tmp6, tmp9);

	tmp7 = _mm_slli_epi32(tmp3, 31);
	tmp8 = _mm_slli_epi32(tmp3, 30);
	tmp9 = _mm_slli_epi32(tmp3, 25);

	tmp7 = _mm_xor_si128(tmp7, tmp8);
	tmp7 = _mm_xor_si128(tmp7, tmp9);
	tmp8 = _mm_srli_si128(tmp7, 4);
	tmp7 = _mm_slli_si128(tmp7, 12);
	tmp3 = _mm_xor_si128(tmp3, tmp7);

	tmp2 = _mm_srli_epi32(tmp3, 1);
	tmp4 = _mm_srli_epi32(tmp3, 2);
	tmp5 = _mm_srli_epi32(tmp3, 7);
	tmp2 = _mm_xor_si128(tmp2, tmp4);
	tmp2 = _mm_xor_si128(tmp2, tmp5);
	tmp2 = _mm_xor_si128(tmp2, tmp8);
	tmp3 = _mm_xor_si128(tmp3, tmp2);
	tmp6 = _mm_xor_si128(tmp6, tmp3);
	
	// GCM byte-swap après multiplication
	tmp6 = _mm_shuffle_epi8(tmp6, BSWAP_MASK);

	_mm_storeu_si128((__m128i*)r, tmp6);
}


/*
 *
#include <stdio.h>
#include <string.h>
int main(int argc, char* argv[])
{
    // high nibble is 0x01, B's high nibble is 0x02
    uint8_t a[16] = {0x1f,0x1e,0x1d,0x1c,0x1b,0x1a,0x19,0x18,0x17,0x16,0x15,0x14,0x13,0x12,0x11,0x10};
    uint8_t b[16] = {0x2f,0x2e,0x2d,0x2c,0x2b,0x2a,0x29,0x28,0x27,0x26,0x25,0x24,0x23,0x22,0x21,0x20};
	//uint8_t a[0x10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	//uint8_t b[0x10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t r[16];

    gfmul(a, b, r);

    // 020BBEB352AEAE16...
    printf("GHASH of message: ");
    printf("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n",
        r[0] & 0xFF, r[1] & 0xFF, r[2] & 0xFF, r[3] & 0xFF,
        r[4] & 0xFF, r[5] & 0xFF, r[6] & 0xFF, r[7] & 0xFF, r[8] & 0xFF, r[9] & 0xFF, r[10] & 0xFF,
		r[11] & 0xFF, r[12] & 0xFF, r[13] & 0xFF, r[14] & 0xFF, r[15] & 0xFF);

    int success = (r[0] == 0x02 && r[1] == 0x0B && r[2] == 0xBE && r[3] == 0xB3 &&
        r[4] == 0x52 && r[5] == 0xAE && r[6] == 0xAE && r[7] == 0x16);

    if (success)
        printf("Success!\n");
    else
        printf("Failure!\n");

    return (success != 0 ? 0 : 1);
}
*/
