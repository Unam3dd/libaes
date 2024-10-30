/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 13:38:14 by stales            #+#    #+#             */
/*   Updated: 2024/10/30 08:36:37 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"

/**
* @prototype round_constant
*
* @brief  This function get round constant
*
* 	This function get round constant
* 	corresponding to the index pass by index
* 	parameter.
*
* @return int          Return value of round constant.
*
* @note		
* 		
* 		where RC[i] is an eight-bit value defined as:
* 			
* 			if i == 1:
* 				return 1
*
* 			if i > 1 and rc[i - 1] < 0x80:
* 				return 2 * rc[i - 1]
*
* 			if i > 1 and rc[i - 1] >= 0x80:
* 				return 2 * rc[i - 1] XOR 0x11B
*
*
* 		where the bits of rc[i] are treated
* 		as the coefficients of an element
* 		of the finite field GF(2)[x]/(x^8 + x^4 + x^3 + x + 1).
*
* 		so that r[10] = 0x36 = 0b110110 represent the polynomial (x^5, x^4, x^2, x).
*
* 		If you are not familiar with Galois Field keep in mind the following
* 		definition:
* 			A Galois Field has a finite number of elements. 
* 			For example, GF(2) has just two elements: 0 and 1. 
* 			GF(2^8) has 256 elements (ranging from 0 to 255 in decimal, or 0x00 to 0xFF in hexadecimal).
*
* 			Addition and Multiplication in GF(2):
* 				Arithmetic in GF(2) (used as the base for larger Galois Fields) is done using mod 2 arithmetic:
* 					Addition: This is just XOR. For example, 1 + 1 = 0 (since 1 XOR 1 = 0).
* 					Multiplication: This is straightforward binary multiplication. For example, 1 × 1 = 1, 1 × 0 = 0.
*
* 			GF(2^8):
* 			In AES, operations are performed in GF(2^8), 
* 			which is a field with 256 elements. 
* 			Each element in GF(2^8) can be represented as an 8-bit byte. 
* 			In this field, arithmetic follws special rules, particularly for multiplication.
*
* 			In GF(2^8), each byte (8 bits) can be treated as a polynomial of degree 7, with coefficients of 0 or 1. 
* 			For example, the byte 11001010 in binary can be seen as the polynomial.
*
* 			(1 * x^7) + (1 * x^6) + (0 * x^5) + (0 * x^4) + (1 * x^3) + (0 * x^2) + (1 * x) + 0
* 			
*
*
*/
rc_t	round_constant(uint8_t i)
{
	static const rc_t RC[] = {
		0x00, 0x01, 0x02, 
		0x04, 0x08, 0x10,
		0x20, 0x40, 0x80, 
		0x1b, 0x36, 0x6c,
		0xd8, 0xab, 0x4d
	};

	return (i < sizeof(RC) ? RC[i] : 0);
}
