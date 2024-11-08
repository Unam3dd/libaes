/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_128_expansion.c                          |    |  |   |   |     |_    */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:11:52 by stales            #+#    #+#             */
/*   Updated: 2024/11/08 14:15:57 by stales              1993-2024            */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"

#if defined (__AES__) && defined(__SSE2__)
#include "./aes_ni_128_expansion.c"
#endif

/**
* @prototype aes_128_key_expansion
*
* @brief  This function Initialize
* each key for each round from cipher key
* with AES Algorithm expansion for
* AES-128 Bits So 10 + 1 First Round
* Total Rounds = 11 (if you count the first)
*
* @param[in] const aes_key_t*: key AES Cipher Key.
*
* @return aes_status_t    Returns AES_OK if success.
* 						Otherwise AES ERR is Returned.
*/

aes_status_t	aes_128_key_expansion(const aes_key_t *k)
{
	if (!k) return (AES_ERR);

	if (check_cpu_support_aes())
		AES_NI_128_KEY_EXPANSION(k->key_128, (aes_round_key_t *)k->sched_128);

	return (AES_OK);
}
