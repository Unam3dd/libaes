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
	printf("=== Test gfmul avec vecteurs connus ===\n\n");
	
	// Test 1: H from NIST test case 4
	uint8_t h[16] = {
		0xb8, 0x3b, 0x53, 0x37, 0x08, 0xbf, 0x53, 0x5d,
		0x0a, 0xa6, 0xe5, 0x29, 0x80, 0xd5, 0x3b, 0x78
	};
	
	// Test 2: Premier bloc AAD from NIST
	uint8_t aad1[16] = {
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef
	};
	
	uint8_t result[16];
	
	printf("Test 1: AAD[0] * H\n");
	print_hex("AAD[0]:  ", aad1, 16);
	print_hex("H:       ", h, 16);
	
	gfmul(aad1, h, result);
	
	print_hex("Result:  ", result, 16);
	
	// Expected result après AAD[0] * H (calculé manuellement ou avec implémentation de référence)
	// Ce serait bien d'avoir un vecteur de test connu pour comparer
	
	printf("\n");
	
	// Test 2: multiplication de 0 par H devrait donner 0
	uint8_t zero[16] = {0};
	
	printf("Test 2: 0 * H = 0\n");
	print_hex("Input:   ", zero, 16);
	
	gfmul(zero, h, result);
	
	print_hex("Result:  ", result, 16);
	print_hex("Expected:", zero, 16);
	
	if (memcmp(result, zero, 16) == 0) {
		printf("✅ Test PASSED\n");
	} else {
		printf("❌ Test FAILED\n");
	}
	
	printf("\n");
	
	// Test 3: H * H
	printf("Test 3: H * H\n");
	print_hex("H:       ", h, 16);
	
	gfmul(h, h, result);
	
	print_hex("H²:      ", result, 16);
	
	return 0;
}

