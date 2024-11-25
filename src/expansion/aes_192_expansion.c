/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes_192_expansion.c                          |    |  |   |   |     |_    */
/*                                                    +:+ +:+         +:+     */
/*   By: stales <stales@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:11:52 by stales            #+#    #+#             */
/*   Updated: 2024/11/12 21:36:38 by stales           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "aes.h"

#if defined (__AES__) && defined(__SSE2__)
#include "./aes_ni_192_expansion.c"
#endif

/**
* @prototype aes_192_key_expansion
*
* @brief  This function Initialize
* each key for each round from cipher key
* with AES Algorithm expansion for
* AES-192 Bits So 10 + 1 First Round
* Total Rounds = 11 (if you count the first)
*
* @param[in] const aes_key_t*: key AES Cipher Key.
*
* @return aes_status_t    Returns AES_OK if success.
* 						Otherwise AES ERR is Returned.
*/

aes_status_t	aes_192_key_expansion(const aes_key_t *k)
{
	if (!k) return (AES_ERR);

	if (cpu_support_aes())
		AES_NI_192_KEY_EXPANSION(k->key_192, (aes_round_key_t *)k->sched_192);

	return (AES_OK);
}
