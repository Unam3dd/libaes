/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 09:45:21 by stales            #+#    #+#             */
/*   Updated: 2024/11/08 13:48:41 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AES_H
#define AES_H

#if !defined (__x86_64__)
#error "This library can be used only on x86 architecture because is used AES-NI instruction set !"
#endif

/////////////////////////////////////
//
//
//			INCLUDES
//
//
////////////////////////////////////

#include <stdint.h>
#include <stddef.h>

/////////////////////////////////////
//
//
//			ALIGN
//
//
////////////////////////////////////

#ifndef ALIGN_BLOCK
#define ALIGN_BLOCK(sz) ((sz + 0x10) & ~0xF)
#endif

/////////////////////////////////////
//
//
//			DEFINES
//
//
////////////////////////////////////

#ifndef AES_128_NR
#define AES_128_NR 10
#else
#undef AES_128_NR
#define AES_128_NR 10
#endif

#ifndef AES_192_NR
#define AES_192_NR 12
#else
#undef AES_192_NR
#define AES_192_NR 12
#endif

#ifndef AES_256_NR
#define AES_256_NR 14
#else
#undef AES_256_NR
#define AES_256_NR 14
#endif

#ifndef AES_KEY_ROUND_SIZE
#define AES_KEY_ROUND_SIZE 0x10
#else
#undef AES_KEY_ROUND_SIZE
#define AES_KEY_ROUND_SIZE 0x10
#endif

/////////////////////////////////////
//
//
//			TYPEDEFS
//
//
////////////////////////////////////

typedef unsigned char	byte_t;
typedef unsigned short	word_t;
typedef unsigned int	dword_t;
typedef unsigned long	qword_t;
typedef const char*		string_t;

/**
*
* @brief  Initialization vector
*
*	There is Initialization vector
*	is just 16 bytes array.
*/

typedef byte_t			iv_t[16];


/**
*
* @brief  Round constant
*/
typedef byte_t	rc_t;

/*
 * @brief 128 bit AES block
 */
typedef byte_t			aes_block_t	__attribute__((vector_size(16), aligned(16)));

/*
 * @brief AES Round Key 16 bytes interpreted as unsigned char
 */
typedef byte_t			aes_round_key_u8_t	__attribute__ ((vector_size(16), aligned(1)));

/*
 * @brief AES Round Key 
 */
typedef long long 		aes_round_key_t	__attribute__ ((vector_size(16), aligned(16)));


typedef struct	s_aes_ctx_t		aes_ctx_t;
typedef struct	s_aes_buf_t		aes_buf_t;
typedef struct	s_aes_key_t		aes_key_t;

/////////////////////////////////////
//
//
//			ENUM
//
//
////////////////////////////////////

typedef enum e_aes_key_size_t
{
	AES_KEY_128 = 0x10,		// 128 bits (16 bytes)
	AES_KEY_192 = 0x18,		// 192 bits (24 bytes)
	AES_KEY_256 = 0x20,		// 256 bits (32 bytes)
} aes_key_size_t;

typedef enum e_aes_modes_t
{
	AES_128_ECB,		/* Electronic Code Books */
	AES_192_ECB,		/* Electronic Code Books */
	AES_256_ECB,		/* Electronic Code Books */
	AES_128_CBC,		/* Cipher Block Chaining */
	AES_192_CBC,		/* Cipher Block Chaining */
	AES_256_CBC,		/* Cipher Block Chaining */
	AES_128_CFB,		/* Cipher Feedback 		 */
	AES_192_CFB,		/* Cipher Feedback 		 */
	AES_256_CFB,		/* Cipher Feedback 		 */
	AES_128_OFB,		/* Output Feedback		 */
	AES_192_OFB,		/* Output Feedback		 */
	AES_256_OFB,		/* Output Feedback		 */
	AES_128_CTR,		/* Counter Mode			 */
	AES_192_CTR,		/* Counter Mode			 */
	AES_256_CTR,		/* Counter Mode			 */
	AES_128_CTS,		/* CipherText Stealing	 */
	AES_192_CTS,		/* CipherText Stealing	 */
	AES_256_CTS,		/* CipherText Stealing	 */
	AES_128_GCM,		/* Galois Counter Mode	 */
	AES_192_GCM,		/* Galois Counter Mode	 */
	AES_256_GCM,		/* Galois Counter Mode	 */
} aes_modes_t;

typedef enum e_aes_status_t
{
	AES_OK,		/*  Ok  */
	AES_ERR		/* Error */
} aes_status_t;

typedef enum e_bool_t
{
	FALSE,
	TRUE
} bool_t;

/////////////////////////////////////
//
//
//			STRUCT
//
//
////////////////////////////////////

struct s_aes_key_t
{
	union {
		
		/* Cipher Key for AES-256 	*/
		byte_t	key_256[AES_KEY_256];
		
		/* Cipher Key for AES-192   */
		byte_t	key_192[AES_KEY_192];
		
		/* Cipher Key for AES-128	*/
		byte_t	key_128[AES_KEY_128];
	};

	union {
		
		/* AES Key Scheduler buffer for AES-256 + 1 For the round 0  */
		aes_round_key_t	sched_256[AES_256_NR + 1];
		byte_t			buf_sched_256[AES_KEY_ROUND_SIZE * (AES_256_NR + 1)];
		
		/* AES Key Scheduler Buffer for AES-192 + 1 For the round 0 */
		aes_round_key_t	sched_192[AES_192_NR + 1]; 
		byte_t			buf_sched_192[AES_KEY_ROUND_SIZE * (AES_192_NR + 1)];
		
		/* AES Key Scheduler Buffer for AES-128 + 1 For the round 0 */
		aes_round_key_t	sched_128[AES_128_NR + 1];
		byte_t			buf_sched_128[AES_KEY_ROUND_SIZE * (AES_128_NR + 1)];
	};
};

struct s_aes_ctx_t
{
	aes_key_t		key;		/* Key & Key Scheduler 		*/
	aes_key_size_t	key_size;	/* Size of the key			*/
	aes_modes_t		mode;		/* Mode used in the context */
};

struct s_aes_buf_t
{
	union {
		string_t	*str;	/* Value as string 			*/
		byte_t		*buf;	/* Value as byte 			*/
		uint64_t	*u64;	/* Value as unsigned long   */
		int64_t		*i64;	/* Value as signed long		*/
		uint32_t	*u32;	/* Value as unsigned int	*/
		int32_t		*i32;	/* Value as signed int		*/
		uint16_t	*u16;	/* Value as unsigned short	*/
		int16_t		*i16;	/* Value as signed short	*/
		uint8_t		*u8;	/* Value as unsigned char	*/
		int8_t		*i8;	/* Value as signed char		*/
	};

	union {
		size_t	len;		/*  Size of the buffer */
		size_t	size;		/*  Size of the buffer */
		size_t	capacity;	/*  Size of the buffer */
	};
};

/////////////////////////////////////
//
//
//	     		CPUID 
//
//
////////////////////////////////////

/**
* @prototype check_cpu_support_aes
*
* @brief  That inline function determine 
* 	if your CPU support the AES-NI instruction set
*
*	This function use cpuid x86 assembly instruction
*	to check if AES-NI is supported by the CPU.
*
*	Related to this wikipedia page
*   https://en.wikipedia.org/wiki/CPUID	
*
*   When you pass 1 to eax register and call cpuid instruction.
*
*   That will be overwrite ecx and edx register.
*   The value of ecx and edx register represent
*   a bit suite of the features supported by the cpu.
*
*   In our case we want check if cpu have aes-ni instruction set
*   so if we look at the wikipedia page you can see the 25 th bits
*   is used to determine if aes-ni is present or not
*   so if we check them with AND bitwise operation
*   you can check if aes-ni is supported
*   that function convert just the AND bitwise mask
*   in boolean
*
*   if ecx & (1 << 25) return (1 << 25) then the
*   cpu support AES-ni and return TRUE otherwise
*   the function return FALSE
*
* @return bool_t	return TRUE or FALSE depending on the CPU.
*/

static inline bool_t	check_cpu_support_aes(void)
{
	uint32_t	ecx = 0;

	__asm__ __volatile__("cpuid\n\t"
			: "=c" (ecx)
			: "a" (1)
			);

	return ((ecx & (1 << 25)) ? TRUE : FALSE);
}

/////////////////////////////////////
//
//
//			ROUND CONSTANT
//
//
////////////////////////////////////

rc_t				round_constant(uint8_t index);

/////////////////////////////////////
//
//
//			AES KEY EXPANSION
//
//
////////////////////////////////////

aes_status_t		aes_128_key_expansion(const aes_key_t *k);
aes_status_t		aes_192_key_expansion(const aes_key_t *k);
aes_status_t		aes_256_key_expansion(const aes_key_t *k);

/////////////////////////////////////
//
//
//	  Electronic Code Books (ECB)
//
//
////////////////////////////////////

aes_status_t	aes_128_ecb_enc(byte_t *out, size_t o_sz, const byte_t *restrict in, size_t i_sz, const aes_ctx_t *ctx);
aes_status_t	aes_128_ecb_dec(byte_t *out, size_t o_sz, const byte_t *restrict in, size_t i_sz, const aes_ctx_t *ctx);

/////////////////////////////////////
//
//
//			MACRO'S
//
//
////////////////////////////////////

#ifndef RC
#define RC(i) (round_constant(i))
#else
#undef RC
#define RC(i) (round_constant(i))
#endif

#endif
