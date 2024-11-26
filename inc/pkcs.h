/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pkcs.h                                       |    |  |   |   |     |_    */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:06:01 by stales            #+#    #+#             */
/*   Updated: 2024/11/26 17:00:30 by stales              1993-2024            */
/*                                                                            */
/* ************************************************************************** */

#ifndef PKCS_H
#define PKCS_H

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
//			DEFINES
//
//
////////////////////////////////////

#ifndef PKCS_SHOULD_PAD
#define PKCS_SHOULD_PAD(blk, size) (size & ~-blk)
#endif

#ifndef PKCS_PAD_LEN
#define PKCS_PAD_LEN(blk, size) (PKCS_SHOULD_PAD(blk, size) ? (blk - (size & ~-blk)) : 0)
#endif

/////////////////////////////////////
//
//
//			ENUM
//
//
////////////////////////////////////

typedef enum e_pkcs_status_t
{
	PKCS_OK,
	PKCS_ERR_BUF_NULL,
	PKCS_ERR_BUF_TOO_SMALL,
	PKCS_ERR_INVALID_SIZE,
	PKCS_ERR_INVALID_BLK_SIZE,
	PKCS_ERR_BUFF_SIZE_ZERO
} pkcs_status_t;

/////////////////////////////////////
//
//
//			STRUCT
//
//
////////////////////////////////////

typedef struct pkcs_buf_t
{
	uint8_t         *buf;       /* pkcs buffer use to pad the data  */
	size_t			size;       /* Size of the buffer               */
    size_t          nb;         /* Number of non zero bytes         */
	size_t			block;      /* Size of the block                */
	size_t			pad_size;   /* Padding size                     */
} pkcs_buf_t;

pkcs_status_t	pkcs_pad(pkcs_buf_t *p);
pkcs_status_t	pkcs_unpad(pkcs_buf_t *p);

#endif
