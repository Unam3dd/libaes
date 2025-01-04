#include "gf.h"
#include <immintrin.h>
#include <smmintrin.h>
#include <emmintrin.h>
#include <wmmintrin.h>

__m128i gf128_mul(__m128i a, __m128i b)
{
	(void)a, (void)b;
	return (_mm_setzero_si128());
}
