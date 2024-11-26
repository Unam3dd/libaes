/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pkcs.c                                       |    |  |   |   |     |_    */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 14:09:21 by stales            #+#    #+#             */
/*   Updated: 2024/11/26 17:58:21 by stales              1993-2024            */
/*                                                                            */
/* ************************************************************************** */

#include "pkcs.h"
#include <stdio.h>
#include <stdint.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>

static size_t len_bytes(const uint8_t *bytes, size_t size)
{
    size_t i = 0;

    while (i < size && bytes[i])
        i++;
    return (i);
}

pkcs_status_t	pkcs_pad(pkcs_buf_t *buf)
{

	if (!buf || !buf->buf)
		return (PKCS_ERR_BUF_NULL);

	if (!buf->size)
		return (PKCS_ERR_BUFF_SIZE_ZERO);

	if (!buf->block || (buf->block > buf->size))
		return (PKCS_ERR_INVALID_BLK_SIZE);

    buf->nb = len_bytes(buf->buf, buf->size);

    if (!PKCS_SHOULD_PAD(buf->block, buf->nb))
		return (PKCS_OK);

	buf->pad_size = PKCS_PAD_LEN(buf->block, buf->nb);

	if ((size_t)(buf->nb + buf->pad_size) > buf->size)
		return (PKCS_ERR_BUF_TOO_SMALL);

	memset((uint8_t*)&buf->buf[buf->nb], buf->pad_size, buf->pad_size);

	return (PKCS_OK);
}

pkcs_status_t	pkcs_unpad(pkcs_buf_t* buf)
{
    uint8_t *ptr = NULL;

    if (!buf || !buf->buf)
        return (PKCS_ERR_BUF_NULL);

    if (!buf->size)
        return (PKCS_ERR_BUFF_SIZE_ZERO);
    
    if (!buf->pad_size)
        return (PKCS_OK);

    if (!buf->block || (buf->block > buf->size) || (buf->pad_size > buf->block) )
        return (PKCS_ERR_INVALID_BLK_SIZE);

    ptr = memrchr(buf->buf, buf->pad_size, buf->size);

    if (!ptr)
        return (PKCS_ERR_BUF_NULL);

    ptr -= ~-buf->pad_size;

    memset((uint8_t *)ptr, 0, buf->pad_size);

	return (PKCS_OK);
}
