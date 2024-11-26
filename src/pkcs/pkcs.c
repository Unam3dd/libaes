/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pkcs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:09:21 by stales            #+#    #+#             */
/*   Updated: 2024/11/26 20:57:42 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pkcs.h"
#include <stdint.h>
#include <string.h>

/**
* @prototype pkcs_pad
*
* @brief  This function pad data following PKCS Padding 
*
*
* @param[in] uint8_t:	buf	  	buffer.
* @param[in] size_t:	size	size.
* @param[in] size_t:	block	size of the block use to determine padding.
*
* @exemple: pkcs_pad(buf, sizeof(buf), 0x10)
*
* @return pkcs_status_t	return PKCS_OK if success. 
* 		Otherwise is an error.
*/

pkcs_status_t	pkcs_pad(uint8_t *buf, size_t size, size_t block)
{
	size_t nb = 0;
	size_t pad = 0;

	if (!buf) return (PKCS_ERR_BUF_NULL);

	if (!size) return (PKCS_ERR_INVALID_SIZE);

	if (!block || block > size) return (PKCS_ERR_INVALID_BLK_SIZE);

	while (nb < size && buf[nb])
		nb++;

	pad = block - (nb & ~-block);

	if (pad)
		memset((uint8_t *)(buf + nb), pad, pad);

	return (PKCS_OK);
}

/**
* @prototype pkcs_unpad
*
* @brief  This function is used to unpad the data
* padded with pkcs_pad function 
* always following pkcs padding.
*
*
* @param[in] uint8_t:	buf		buffer contained data to unpad.
* @param[in] size_t:	size	size of the buffer.
* @param[in] size_t:	block	size of the block.
*
* @return pkcs_status_t	return PKCS_OK if success.
* 						otherwise is considered as error.
*/

pkcs_status_t	pkcs_unpad(uint8_t *buf, size_t size, size_t block)
{
	uint8_t *ptr = NULL;

	if (!buf)
		return (PKCS_ERR_BUF_NULL);

	if (!size)
		return (PKCS_ERR_INVALID_BLK_SIZE);

	if (!block || block > size)
		return (PKCS_ERR_INVALID_BLK_SIZE);

	ptr = (uint8_t *)(buf + (size - 1));

	while (*ptr == 0)
		--ptr;

	memset((uint8_t *)(ptr - *ptr) + 1, 0, *ptr);

	return ((uint64_t)(((ptr - *ptr) + 1) - buf) == (block - *ptr) 
			? PKCS_OK 
			: PKCS_ERR_INVALID_BLK_SIZE);
}
