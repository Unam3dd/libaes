/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pkcs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:09:21 by stales            #+#    #+#             */
/*   Updated: 2024/10/20 08:44:56 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pkcs.h"
#include <string.h>
#include <stdint.h>

/**
* @prototype pkcs_pad
*
* @brief  That function pad data with related to the size of the block.
* That function follow the PKCS#5/PKCS#7 specification.
*
* @param[in] uint8_t*:		buf			Buf contain data.
* @param[in] size_t:		buf_size	Size of the buffer.
* @param[in] size_t:		size		Number of bytes in buffer.
* @param[in] uint8_t:		blk			size of the block.
* @return pkcs_status_t		status		PKCS_OK if success otherwise is an error.
*
* @Note If the first argument is NULL a PKCS_ERR_BUF_NULL is returned.
* @Note If the size of the buffer is zero then PKCS_ERR_BUFF_SIZE_ZERO is returned.
* @Note If the size of the block is zero then PKCS_ERR_INVALID_BLK_SIZE is returned.
* @Note If the size is greater than buffer size PKCS_ERR_INVALID_SIZE is returned.
* @Note If the size and the size of the padding is bigger than buffer_size then PKCS_ERR_BUF_TOO_SMALL
* is returned.
*/

pkcs_status_t	pkcs_pad(uint8_t *buf, size_t buf_size, size_t size, uint8_t blk)
{
	uint8_t pad = 0;

	if (!buf)
		return (PKCS_ERR_BUF_NULL);

	if (!buf_size)
		return (PKCS_ERR_BUFF_SIZE_ZERO);

	if (!blk)
		return (PKCS_ERR_INVALID_BLK_SIZE);

	if (size > buf_size)
		return (PKCS_ERR_INVALID_SIZE);

	pad = PKCS_PAD_LEN(blk, size);

	if ((size_t)(size + pad) >= buf_size)
		return (PKCS_ERR_BUF_TOO_SMALL);

	memset((uint8_t*)&buf[size], pad, pad);

	return (PKCS_OK);
}

/**
* @prototype pkcs_unpad
*
* @brief  pkcs_unpad like function pkcs_pad just above allow
* to unpad the data following PKCS#5/PKCS#7 specification.
*
*	This function unpad the data padded with pkcs_pad
*
* @param[in] uint8_t*:		buf			Buf contain data.
* @param[in] size_t:		buf_size	Size of the buffer.
* @param[in] size_t:		size		Number of bytes in buffer.
* @param[in] uint8_t:		blk			size of the block.
* @return pkcs_status_t		status		PKCS_OK if success otherwise is an error.
*
* @Note If the first argument is NULL a PKCS_ERR_BUF_NULL is returned.
* 
* @Note If the size of the buffer is zero then PKCS_ERR_BUFF_SIZE_ZERO is returned.
* 
* @Note If the size of the block is zero or the number of bytes in the buffer is not multiple of the block 
* 		then PKCS_ERR_INVALID_BLK_SIZE is returned.
*
* @Note If the size is greater than buffer size PKCS_ERR_INVALID_SIZE is returned.
*/


pkcs_status_t	pkcs_unpad(uint8_t *buf, size_t buf_size, size_t size, uint8_t blk)
{
	uint8_t	*tmp = NULL;
	uint8_t pad = 0;

	if (!buf)
		return (PKCS_ERR_BUF_NULL);
	
	if (!buf_size)
		return (PKCS_ERR_INVALID_SIZE);

	if (size > buf_size)
		return (PKCS_ERR_INVALID_SIZE);

	if (!blk || (size & ~-blk))
		return (PKCS_ERR_INVALID_BLK_SIZE);

	if (!size)
		return (PKCS_OK);

	tmp = (uint8_t*)(buf + (size - 1));
	pad = *tmp;

	while (*tmp == pad)
		*tmp-- = 0;

	return (PKCS_OK);
}
