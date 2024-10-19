/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pkcs.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:06:01 by stales            #+#    #+#             */
/*   Updated: 2024/10/19 20:12:37 by stales           ###   ########.fr       */
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

#ifndef PKCS_PAD_LEN
#define PKCS_PAD_LEN(blk, size) (blk - (size & ~-blk))
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

pkcs_status_t	pkcs_pad(uint8_t *buf, size_t buf_size, size_t size, uint8_t blk);
pkcs_status_t	pkcs_unpad(uint8_t *buf, size_t buf_size, size_t size, uint8_t blk);

#endif
