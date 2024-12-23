#include "gf.h"
#include <immintrin.h>
#include <smmintrin.h>
#include <emmintrin.h>

__m128i gf128_mul(__m128i a, __m128i b)
{
	__m256i poly = _mm256_set_epi64x(0, 0x1, 0, 0x87);

	__m256i res = _mm256_setzero_si256();
	__m256i aa = _mm256_load_si256((__m256i*)&a);
	__m256i bb = _mm256_setzero_si256();

	_mm256_xor_si256(res, res);

	__m256i tmp5 =  _mm256_clmulepi64_epi128(a, b, 0x0);
	__m256i tmp4 = _mm256_clmulepi64_epi128(a, b, 0x10);
	__m256i tmp3 = _mm256_clmulepi64_epi128(a, b, 0x01);
	__m256i tmp2 = _mm256_clmulepi64_epi128(a, b, 0x11);

	tmp5 = _mm256_xor_si256(tmp5, tmp3);
	tmp2 = _mm256_xor_si256(tmp2, tmp4);

	//res = _mm256_clmulepi64_si128(tmp5, poly, 0x10);

	//tmp3 = _mm_shuffle_epi32(tmp5, 0x4e);

	res = _mm256_xor_si256(res, tmp2);
	res = _mm256_xor_si256(res, tmp3);

	return (((__m128i*)&res)[0]);
}
