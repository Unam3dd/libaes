#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>

static void print_hex(const char *label, const unsigned char *buf, size_t size)
{
	printf("%s", label);
	for (size_t i = 0; i < size; i++) {
		printf("%02x", buf[i]);
	}
	printf("\n");
}

int main(void)
{
	unsigned char key[16] = {
		0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c,
		0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08
	};
	
	// IV de 96 bits from NIST
	unsigned char iv[12] = {
		0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad,
		0xde, 0xca, 0xf8, 0x88
	};
	
	unsigned char plaintext[64] = {
		0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5,
		0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
		0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda,
		0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
		0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53,
		0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
		0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57,
		0xba, 0x63, 0x7b, 0x39, 0x1a, 0xaf, 0xd2, 0x55
	};
	
	unsigned char aad[20] = {
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
		0xab, 0xad, 0xda, 0xd2
	};
	
	unsigned char ciphertext[64];
	unsigned char tag[16];
	int outlen;
	
	printf("=== Comparaison avec différentes longueurs de tag ===\n\n");
	
	print_hex("Key:        ", key, 16);
	print_hex("IV (12 bytes): ", iv, 12);
	print_hex("AAD:        ", aad, 20);
	printf("\n");
	
	// Test avec tag de 16 bytes
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
	
	EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL);
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, NULL);
	EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);
	
	// Ajouter AAD
	EVP_EncryptUpdate(ctx, NULL, &outlen, aad, 20);
	
	// Chiffrer
	int len1, len2;
	EVP_EncryptUpdate(ctx, ciphertext, &len1, plaintext, 64);
	EVP_EncryptFinal_ex(ctx, ciphertext + len1, &len2);
	
	// Récupérer tag
	EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag);
	
	print_hex("Ciphertext: ", ciphertext, 64);
	print_hex("Tag (16):   ", tag, 16);
	
	printf("\nAttendu NIST:\n");
	printf("Ciphertext: 42831ec2217774244b7221b784d0d49ce3aa212f2c02a4e035c17e2329aca12e21d514b25466931c7d8f6a5aac84aa051ba30b396a0aac973d58e091473f5985\n");
	printf("Tag:        4d5c2af327cd64a62cf35abd2ba6fab4\n");
	
	EVP_CIPHER_CTX_free(ctx);
	
	return 0;
}
