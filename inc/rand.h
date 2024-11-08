/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rand.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 18:12:48 by stales            #+#    #+#             */
/*   Updated: 2024/11/07 18:22:36 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAND_H
#define RAND_H

#include <stddef.h>
#include <stdint.h>

/**
* @prototype urandom
*
* @brief  Write random bytes from /dev/urandom
* into the buffer pass in parameters.
*
*	There is simple description 
*
* @param[in] uint8_t *restirct: buffer	   The output buffer where you want write random bytes.
* @param[in] size_t: size				   The number of bytes you want write in the buffer.
* @return uint8_t *						   Return the buffer pass in parameters.
*/

uint8_t	*urandom(uint8_t *restrict buffer, size_t size);

#if defined(__RDRND__)
uint8_t	*rdrnd(uint8_t *restrict buffer, size_t size);
#endif

#endif
