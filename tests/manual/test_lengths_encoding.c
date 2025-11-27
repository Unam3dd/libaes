#include <stdio.h>
#include <stdint.h>
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
	// Test encodage des longueurs pour NIST test case
	// AAD = 20 bytes = 160 bits
	// Ciphertext = 64 bytes = 512 bits
	
	size_t aad_len = 20;
	size_t ct_len = 64;
	
	uint64_t aad_bits = aad_len * 8;  // 160
	uint64_t ct_bits = ct_len * 8;     // 512
	
	printf("=== Test Encodage Longueurs ===\n\n");
	printf("AAD Length:  %zu bytes = %lu bits\n", aad_len, aad_bits);
	printf("CT Length:   %zu bytes = %lu bits\n\n", ct_len, ct_bits);
	
	// Méthode actuelle (votre code)
	uint8_t lengths_v1[16] = {0};
	for (size_t i = 0; i < 8; i++) {
		lengths_v1[7 - i] = (aad_bits >> (i * 8)) & 0xFF;
		lengths_v1[15 - i] = (ct_bits >> (i * 8)) & 0xFF;
	}
	
	print_hex("Encodage actuel:  ", lengths_v1, 16);
	
	// Méthode standard big-endian
	uint8_t lengths_v2[16] = {0};
	// AAD length (bits 0-63)
	lengths_v2[0] = (aad_bits >> 56) & 0xFF;
	lengths_v2[1] = (aad_bits >> 48) & 0xFF;
	lengths_v2[2] = (aad_bits >> 40) & 0xFF;
	lengths_v2[3] = (aad_bits >> 32) & 0xFF;
	lengths_v2[4] = (aad_bits >> 24) & 0xFF;
	lengths_v2[5] = (aad_bits >> 16) & 0xFF;
	lengths_v2[6] = (aad_bits >> 8) & 0xFF;
	lengths_v2[7] = (aad_bits >> 0) & 0xFF;
	// CT length (bits 64-127)
	lengths_v2[8] = (ct_bits >> 56) & 0xFF;
	lengths_v2[9] = (ct_bits >> 48) & 0xFF;
	lengths_v2[10] = (ct_bits >> 40) & 0xFF;
	lengths_v2[11] = (ct_bits >> 32) & 0xFF;
	lengths_v2[12] = (ct_bits >> 24) & 0xFF;
	lengths_v2[13] = (ct_bits >> 16) & 0xFF;
	lengths_v2[14] = (ct_bits >> 8) & 0xFF;
	lengths_v2[15] = (ct_bits >> 0) & 0xFF;
	
	print_hex("Encodage standard: ", lengths_v2, 16);
	
	// Attendu selon NIST pour len(A)=160, len(C)=512
	// 160 en binaire = 0x00000000000000A0
	// 512 en binaire = 0x0000000000000200
	uint8_t expected[16] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0,  // 160 bits
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00   // 512 bits
	};
	
	print_hex("Attendu (NIST):   ", expected, 16);
	printf("\n");
	
	if (memcmp(lengths_v1, expected, 16) == 0) {
		printf("✅ Encodage actuel est CORRECT!\n");
	} else {
		printf("❌ Encodage actuel est INCORRECT!\n");
	}
	
	if (memcmp(lengths_v2, expected, 16) == 0) {
		printf("✅ Encodage standard est CORRECT!\n");
	} else {
		printf("❌ Encodage standard est INCORRECT!\n");
	}
	
	return 0;
}

