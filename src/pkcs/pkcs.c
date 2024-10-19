/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pkcs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:09:21 by stales            #+#    #+#             */
/*   Updated: 2024/10/19 20:25:53 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pkcs.h"
#include <string.h>
#include <stdint.h>

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
