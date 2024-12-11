#include <stdio.h>
#include <stdint.h>

#define POLYNOMIAL 0x87  // Irreducible polynomial for GF(2^128): x^128 + x^7 + x^2 + x + 1

// Function to multiply two 128-bit elements in GF(2^128)
void gf128_multiply(uint8_t a[16], uint8_t b[16], uint8_t result[16]) {
    uint8_t p[16] = { 0 };  // Accumulator for the product (128 bits)
    uint8_t temp_a[16];
    uint8_t temp_b[16];

    // Copy a and b into temporary variables for processing
    for (int i = 0; i < 16; i++) {
        temp_a[i] = a[i];
        temp_b[i] = b[i];
    }

    for (int i = 0; i < 128; i++) {
        if (temp_b[15] & 1) {  // Check if the LSB of b is 1
            for (int j = 0; j < 16; j++) {
                p[j] ^= temp_a[j];  // Add a to the result (XOR in GF(2))
            }
        }

        // Shift b to the right (divide by x) and check if it overflows
        for (int j = 0; j < 15; j++) {
            temp_b[j] = (temp_b[j] >> 1) | ((temp_b[j + 1] & 1) << 7);  // Shift right with carry
        }
        temp_b[15] >>= 1;

        // Check if a has a term for x^127 (the highest bit) and reduce modulo the polynomial if necessary
        if (temp_a[0] & 0x80) {  // If MSB of a is 1, reduce with the irreducible polynomial
            // Shift a left (multiply by x) and reduce by the polynomial
            for (int j = 0; j < 15; j++) {
                temp_a[j] = (temp_a[j] << 1) | (temp_a[j + 1] >> 7);
            }
            temp_a[15] = (temp_a[15] << 1) ^ POLYNOMIAL;  // XOR with the irreducible polynomial
        } else {
            // If MSB of a is 0, just shift left
            for (int j = 0; j < 15; j++) {
                temp_a[j] = (temp_a[j] << 1) | (temp_a[j + 1] >> 7);
            }
            temp_a[15] <<= 1;
        }
    }

    // Copy the result back to the result array
    for (int i = 0; i < 16; i++) {
        result[i] = p[i];
    }
}

// Utility function to print a 128-bit value as hexadecimal
void print_hex(uint8_t data[16]) {
    for (int i = 0; i < 16; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

int main(void) {
    // Example values for multiplication (128-bit values represented as 16 bytes)
    uint8_t a[16] = { 0x87, 0x6f, 0x5c, 0x20, 0xf7, 0x82, 0x7b, 0xb6,
                      0x6a, 0xa7, 0x9e, 0xbe, 0xd7, 0x3a, 0x9f, 0x58 };

    uint8_t b[16] = { 0x77, 0x12, 0x44, 0x67, 0x88, 0x53, 0xe9, 0xd9,
                      0x35, 0x4a, 0xaf, 0x6d, 0xd1, 0x9b, 0x58, 0x12 };

    uint8_t result[16];  // To store the result of multiplication

    gf128_multiply(a, b, result);

    printf("Result: ");
    print_hex(result);

    return 0;
}

