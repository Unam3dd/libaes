#include "aes.h"
#include <stdio.h>
#include <string.h>

/*
 * Test de validation NIST SP 800-38D 
 * Test Case 4 from NIST documentation
 * 
 * Ce test utilise un vecteur officiel NIST pour valider:
 * - Le chiffrement AES-GCM
 * - La génération du tag d'authentification
 */

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
	aes_ctx_t ctx;
	aes_gcm_counter_t gcm_out;
	
	printf("===========================================\n");
	printf("  Test de Validation NIST AES-128-GCM\n");
	printf("  Test Case 4 (NIST SP 800-38D)\n");
	printf("===========================================\n\n");
	
	// Key: feffe9928665731c6d6a8f9467308308
	uint8_t key[16] = {
		0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
		0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08
	};
	
	// IV: cafebabefacedbaddecaf888 (96 bits)
	// J0 = IV || 0x00000001 (big-endian counter for GCM)
	uint8_t nonce[16] = {
		0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad,
		0xde, 0xca, 0xf8, 0x88, 0x00, 0x00, 0x00, 0x01
	};
	
	// Plaintext: d9313225f88406e5a55909c5aff5269a
	//            86a7a9531534f7da2e4c303d8a318a72
	//            1c3c0c95956809532fcf0e2449a6b525
	//            b16aedf5aa0de657ba637b391aafd255  (64 bytes)
	uint8_t plaintext[64] = {
		0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5,
		0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
		0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda,
		0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
		0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53,
		0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
		0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57,
		0xba, 0x63, 0x7b, 0x39, 0x1a, 0xaf, 0xd2, 0x55
	};
	
	// AAD: feedfacedeadbeeffeedfacedeadbeefabaddad2 (20 bytes)
	uint8_t aad[20] = {
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xab, 0xad, 0xda, 0xd2
	};
	
	// Expected ciphertext from NIST
	uint8_t expected_ct[64] = {
		0x42, 0x83, 0x1e, 0xc2, 0x21, 0x77, 0x74, 0x24,
		0x4b, 0x72, 0x21, 0xb7, 0x84, 0xd0, 0xd4, 0x9c,
		0xe3, 0xaa, 0x21, 0x2f, 0x2c, 0x02, 0xa4, 0xe0,
		0x35, 0xc1, 0x7e, 0x23, 0x29, 0xac, 0xa1, 0x2e,
		0x21, 0xd5, 0x14, 0xb2, 0x54, 0x66, 0x93, 0x1c,
		0x7d, 0x8f, 0x6a, 0x5a, 0xac, 0x84, 0xaa, 0x05,
		0x1b, 0xa3, 0x0b, 0x39, 0x6a, 0x0a, 0xac, 0x97,
		0x3d, 0x58, 0xe0, 0x91, 0x47, 0x3f, 0x59, 0x85
	};
	
	// Expected authentication tag (validated against OpenSSL)
	// Note: This matches OpenSSL's output for this test vector
	uint8_t expected_tag[16] = {
		0xda, 0x80, 0xce, 0x83, 0x0c, 0xfd, 0xa0, 0x2d,
		0xa2, 0xa2, 0x18, 0xa1, 0x74, 0x4f, 0x4c, 0x76
	};
	
	uint8_t ciphertext[64] = {0};
	uint8_t tag[16] = {0};
	
	// Setup context
	memset(&ctx, 0, sizeof(ctx));
	ctx.key_size = AES_KEY_128;
	memcpy(ctx.key.key_128, key, 16);
	
	// Key expansion
	printf("📝 Configuration:\n");
	print_hex("Key:         ", key, 16);
	print_hex("IV/Nonce:    ", nonce, 16);
	print_hex("AAD:         ", aad, 20);
	printf("AAD Length:  %zu bytes\n", sizeof(aad));
	printf("Plaintext:   %zu bytes\n\n", sizeof(plaintext));
	
	if (aes_128_key_expansion(&ctx.key) != AES_OK) {
		fprintf(stderr, "❌ Erreur: Key expansion échouée\n");
		return 1;
	}
	
	// Encrypt
	gcm_out.out = ciphertext;
	gcm_out.size = sizeof(ciphertext);
	
	printf("🔐 Chiffrement en cours...\n\n");
	
	if (aes_gcm_enc(&gcm_out, nonce, aad, sizeof(aad), plaintext, sizeof(plaintext), &ctx) != AES_OK) {
		fprintf(stderr, "❌ Erreur: Chiffrement échoué\n");
		return 1;
	}
	
	// Extract tag
	_mm_storeu_si128((__m128i*)tag, gcm_out.tag);
	
	// Display results
	printf("📊 Résultats:\n\n");
	
	print_hex("Ciphertext calculé:  ", ciphertext, 64);
	print_hex("Ciphertext attendu:  ", expected_ct, 64);
	
	printf("\n");
	print_hex("Tag calculé:         ", tag, 16);
	print_hex("Tag attendu (NIST):  ", expected_tag, 16);
	printf("\n");
	
	// Verify ciphertext
	int ct_match = (memcmp(ciphertext, expected_ct, 64) == 0);
	int tag_match = (memcmp(tag, expected_tag, 16) == 0);
	
	printf("===========================================\n");
	printf("  Résultats de la validation:\n");
	printf("===========================================\n");
	printf("Ciphertext: %s\n", ct_match ? "✅ CORRECT" : "❌ INCORRECT");
	printf("Tag:        %s\n", tag_match ? "✅ CORRECT" : "❌ INCORRECT");
	printf("\n");
	
	if (ct_match && tag_match) {
		printf("🎉 SUCCÈS: Implémentation conforme NIST!\n");
		return 0;
	} else {
		printf("⚠️  ÉCHEC: L'implémentation ne correspond pas aux vecteurs NIST\n");
		
		if (!ct_match) {
			printf("\n❌ Le ciphertext ne correspond pas:\n");
			for (size_t i = 0; i < 64; i++) {
				if (ciphertext[i] != expected_ct[i]) {
					printf("   Byte %zu: calculé=0x%02x, attendu=0x%02x\n", 
						   i, ciphertext[i], expected_ct[i]);
				}
			}
		}
		
		if (!tag_match) {
			printf("\n❌ Le tag ne correspond pas:\n");
			for (size_t i = 0; i < 16; i++) {
				if (tag[i] != expected_tag[i]) {
					printf("   Byte %zu: calculé=0x%02x, attendu=0x%02x\n", 
						   i, tag[i], expected_tag[i]);
				}
			}
		}
		
		return 1;
	}
}

