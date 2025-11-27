#include "../../inc/gf.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
	uint8_t zero[16] = {0};
	uint8_t h[16] = {
		0x66, 0xe9, 0x4b, 0xd4, 0xef, 0x8a, 0x2c, 0x3b,
		0x88, 0x4c, 0xfa, 0x59, 0xca, 0x34, 0x2b, 0x2e
	};
	uint8_t result[16];
	
	printf("Test: gfmul(0, H, result)\n");
	printf("Input A (0):  ");
	for (int i = 0; i < 16; i++) printf("%02x", zero[i]);
	printf("\nInput B (H):  ");
	for (int i = 0; i < 16; i++) printf("%02x", h[i]);
	
	gfmul(zero, h, result);
	
	printf("\nResult:       ");
	for (int i = 0; i < 16; i++) printf("%02x", result[i]);
	printf("\nExpected (0): ");
	for (int i = 0; i < 16; i++) printf("%02x", zero[i]);
	printf("\n\n");
	
	if (memcmp(result, zero, 16) == 0) {
		printf("✅ gfmul(0, H) = 0 PASSED\n");
		return 0;
	} else {
		printf("❌ gfmul(0, H) ≠ 0 FAILED\n");
		return 1;
	}
}
