#include "gf.h"
#include <stdio.h>
#include <string.h>

static void print_hex(const char *label, const uint8_t *buf, size_t size)
{
	printf("%s", label);
	for (size_t i = 0; i < size; i++) {
		printf("%02x", buf[i]);
	}
	printf("\n");
}

int main(void)
{
	// Test simple: multiplication par H dans GF(2^128)
	// Si tout va bien, on devrait obtenir le résultat attendu
	
	// Test vecteur simple : 0 * H = 0
	uint8_t zero[16] = {0};
	uint8_t h[16] = {0x66, 0xe9, 0x4b, 0xd4, 0xef, 0x8a, 0x2c, 0x3b,
	                  0x88, 0x4c, 0xfa, 0x59, 0xca, 0x34, 0x2b, 0x2e};
	uint8_t result[16];
	
	printf("=== Test gfmul ===\n");
	print_hex("Input A (0):     ", zero, 16);
	print_hex("Input B (H):     ", h, 16);
	
	gfmul(zero, h, result);
	
	print_hex("Result:          ", result, 16);
	print_hex("Expected (0):    ", zero, 16);
	
	if (memcmp(result, zero, 16) == 0) {
		printf("✅ Test 0*H = 0 PASSED\n\n");
	} else {
		printf("❌ Test 0*H = 0 FAILED\n\n");
	}
	
	// Test: identité
	uint8_t one[16] = {0};
	one[15] = 1; // En little-endian, 1 est à la fin
	
	print_hex("Input A (1):     ", one, 16);
	print_hex("Input B (H):     ", h, 16);
	
	gfmul(one, h, result);
	
	print_hex("Result (1*H):    ", result, 16);
	print_hex("Expected (H):    ", h, 16);
	
	// Note: en GF(2^128), 1*H pourrait ne pas être H selon la représentation
	// C'est juste pour voir ce que donne la fonction
	
	return 0;
}

