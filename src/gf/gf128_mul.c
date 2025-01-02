#include "gf.h"
#include <immintrin.h>
#include <smmintrin.h>
#include <emmintrin.h>
#include <wmmintrin.h>

__m128i gf128_mul(__m128i a, __m128i b)
{
	//const __m256i poly = _mm256_set_epi64x(0, 0x1, 0, 0x87);

	__m128i low_low = _mm_clmulepi64_si128(a, b, 0x0);
	__m128i high_high = _mm_clmulepi64_si128(a, b, 0x11);
	__m128i mid_low = _mm_clmulepi64_si128(a, b, 0x01);
	__m128i mid_high = _mm_clmulepi64_si128(a, b, 0x10);

	_mm_xor_si128(low_low, mid_low);
	_mm_xor_si128(high_high, mid_high);

	return (_mm_setzero_si128());
}
