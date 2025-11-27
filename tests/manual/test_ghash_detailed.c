#include "aes.h"
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

static void print_m128i(const char *label, __m128i val)
{
	uint8_t buf[16];
	_mm_storeu_si128((__m128i*)buf, val);
	print_hex(label, buf, 16);
}

int main(void)
{
	// Test simple: GHASH avec un seul bloc de ciphertext, pas d'AAD
	
	// H from NIST test
	uint8_t h[16] = {
		0xb8, 0x3b, 0x53, 0x37, 0x08, 0xbf, 0x53, 0x5d,
		0x0a, 0xa6, 0xe5, 0x29, 0x80, 0xd5, 0x3b, 0x78
	};
	
	// Premier bloc de ciphertext from NIST
	uint8_t c1[16] = {
		0x42, 0x83, 0x1e, 0xc2, 0x21, 0x77, 0x74, 0x24,
		0x4b, 0x72, 0x21, 0xb7, 0x84, 0xd0, 0xd4, 0x9c
	};
	
	// AAD from NIST (20 bytes)
	uint8_t aad[20] = {
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xab, 0xad, 0xda, 0xd2
	};
	
	printf("=== Test GHASH Détaillé ===\n\n");
	print_hex("H:         ", h, 16);
	print_hex("AAD:       ", aad, 20);
	print_hex("C1:        ", c1, 16);
	printf("\n");
	
	// Calculer GHASH manuellement
	uint8_t ghash[16] = {0};
	uint8_t temp[16];
	
	// Étape 1: Process AAD bloc 1 (16 bytes)
	printf("Étape 1: AAD bloc 1\n");
	memcpy(temp, aad, 16);
	print_hex("  AAD[0]:  ", temp, 16);
	
	// ghash = (ghash XOR AAD[0]) * H
	for (int i = 0; i < 16; i++) ghash[i] ^= temp[i];
	print_hex("  ghash XOR AAD[0]: ", ghash, 16);
	
	gfmul(ghash, h, ghash);
	print_hex("  ghash après mul: ", ghash, 16);
	printf("\n");
	
	// Étape 2: Process AAD partiel (4 bytes)
	printf("Étape 2: AAD bloc partiel\n");
	memset(temp, 0, 16);
	memcpy(temp, aad + 16, 4);
	print_hex("  AAD[1]:  ", temp, 16);
	
	for (int i = 0; i < 16; i++) ghash[i] ^= temp[i];
	print_hex("  ghash XOR AAD[1]: ", ghash, 16);
	
	gfmul(ghash, h, ghash);
	print_hex("  ghash après mul: ", ghash, 16);
	printf("\n");
	
	// Étape 3: Process C1
	printf("Étape 3: Ciphertext bloc 1\n");
	print_hex("  C[0]:    ", c1, 16);
	
	for (int i = 0; i < 16; i++) ghash[i] ^= c1[i];
	print_hex("  ghash XOR C[0]: ", ghash, 16);
	
	gfmul(ghash, h, ghash);
	print_hex("  ghash après mul: ", ghash, 16);
	printf("\n");
	
	// Pour le test complet, il faudrait ajouter tous les blocs de ciphertext
	// puis le bloc de longueurs
	
	printf("Note: Ce test montre juste les premières étapes de GHASH\n");
	printf("Pour un résultat final, il faut traiter tous les 4 blocs de ciphertext\n");
	printf("puis ajouter le bloc de longueurs len(A)||len(C)\n");
	
	return 0;
}

